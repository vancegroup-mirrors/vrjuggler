#ifndef _OSG_NAVIGATER_H
#define _OSG_NAVIGATER_H

#include <gmtl/EulerAngle.h>
#include <gmtl/EulerAngleOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>
#include <osg/MatrixTransform>
#include <vpr/IO/SerializableObject.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <cluster/Plugins/ApplicationDataManager/UserData.h>

/** Navigation data to share across cluster
 */
class NavData : public vpr::SerializableObject
{
public:
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
      float pos_data[16];
      for ( unsigned n=0;n<16;n++ )
      {
         pos_data[n] = reader->readFloat();
      }

      mCurPos.set(pos_data);
      return vpr::ReturnStatus::Succeed;
   }

   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
      const float* pos_data = mCurPos.getData();
      for ( int n=0;n<16;n++ )
      {
         writer->writeFloat(pos_data[n]);
      }
      return vpr::ReturnStatus::Succeed;
   }
public:
   gmtl::Matrix44f mCurPos;
};

/** Class to control all navigation */
class OsgNavigater
{
   /** Navigation mode */
   enum NavMode
   {
      WALK,       /**< Constrain to x,y trans and y rotations. */
      FLY         /**< No restrictions */
   };

public:
   OsgNavigater() : mActive(false), mMode(FLY)
   {;}

   void init()
   {
      mActive = true;
      vpr::GUID new_guid("d6be4359-e8cf-41fc-a72b-a5b4f3f29aa2");
      std::string hostname = "crash";
      mNavData.init(new_guid, hostname);
   }

   void setWalkMode(bool walk_mode)
   {
      if(walk_mode)
      {
         mMode = WALK;
      }
      else
      {
         mMode = FLY;
      }
   }

   void setVelocity(const gmtl::Vec3f& vel)
   {
      mVelocity = vel;
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Trans Vel: " << mVelocity << std::endl << vprDEBUG_FLUSH;
   }
   void setRotationalVelocity(const gmtl::Matrix44f& rot_vel)
   {
      mRotVelocity = rot_vel;
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Trans Vel: " << mRotVelocity << std::endl << vprDEBUG_FLUSH;
   }
   void update(float delta)
   {
      // Don't update if not active or we have a very large delta
      if(delta > 2.0 || !mActive)
      {
         return;
      }
      // Clamp delta
      if (delta > 1.0)
      {  delta = 1.0f; }

      // ---------- ROTATION ----------- //
      // Scale the rotation velocity (slerp) to get a time based change
      gmtl::Quatf qrot = gmtl::make<gmtl::Quatf>(mRotVelocity);
      gmtl::Quatf scaled_qrot, src_rot;   // scaled rotation and Identity rotation
      gmtl::Matrix44f delta_rot;          // The delta rotation to use

      // Only compute if we don't have identity rotation
      if (!(gmtl::isEqual(gmtl::MAT_IDENTITY44F, mRotVelocity, 0.001f))
      {
         gmtl::slerp(scaled_qrot, delta, src_rot, qrot);
         gmtl::set(delta_rot, scaled_qrot);
      }

      if(mMode == WALK)
      {
         gmtl::EulerAngleXYZf euler( 0.0f, gmtl::makeYRot(delta_rot), 0.0f );// Only allow Yaw (rot y)
         delta_rot = gmtl::makeRot<gmtl::Matrix44f>( euler );
      }
      gmtl::postMult(mNavData->mCurPos, delta_rot);

      // ------- TRANSLATION ---- //
      gmtl::Vec3f trans_delta;
      if(mMode == WALK)
      {
         mVelocity[1] = 0.0f;
      }
      trans_delta =  mVelocity * delta;

      // Post multiply the delta translation
      gmtl::Matrix44f trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(trans_delta);
      gmtl::postMult(mNavData->mCurPos, trans_matrix);
   }

   gmtl::Matrix44f getCurPos()
   {
      if(!mActive)
      {
         return gmtl::Matrix44f();
      }
      else
      {
         return mNavData->mCurPos;
      }
   }

   void setCurPos(const gmtl::Matrix44f& pos)
   {
      if(mActive)
      {
         mNavData->mCurPos = pos;
      }
   }

private:
   bool                    mActive;
   NavMode                 mMode;

   /** Current postion (as userdate for sharing across cluster */
   cluster::UserData< NavData >  mNavData;

   gmtl::Vec3f             mVelocity;        /**< Current trans velocity */
   gmtl::Matrix44f         mRotVelocity;     /**< Current angular velocity */
};
#endif /* _OSG_NAVIGATE_H */
