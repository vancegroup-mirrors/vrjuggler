/* Generated by Together */

#ifndef SOUNDIMPLEMENTATION_H
#define SOUNDIMPLEMENTATION_H

class SoundImplementation
{
public:



   /**
    * @semantics default constructor 
    */
   SoundImplementation() {}

   /**
    * @semantics default constructor 
    */
   SoundImplementation( const SoundImplementation& copy )
   {
      this->copy( cp );
   }

   virtual void copy( const SoundImplementation& isi )
   {
      
   }

   /**
    * @semantics destructor 
    */
   virtual ~SoundImplementation() {}
private:
   struct SoundInfo
   {
      
   };
   std::list<SoundInfo> mSounds;
};
#endif //SOUNDIMPLEMENTATION_H