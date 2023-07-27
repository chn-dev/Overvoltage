#include "Sample.h"

Sample::Sample( std::string name, WaveFile *pWave, int minNote, int maxNote ) :
   m_Name( name ),
   m_pWave( pWave ),
   m_MinNote( minNote ),
   m_MaxNote( maxNote ),
   m_BaseNote( minNote ),
   m_PlayMode( Sample::PlayModeStandard ),
   m_DetuneCents( 0.0 ),
   m_Reverse( false ),
   m_MinVelocity( 0 ),
   m_MaxVelocity( 127 )
{
   m_pAEG = new ENV();
}


Sample::~Sample()
{
   delete m_pWave;
   delete m_pAEG;
}


ENV *Sample::getAEG() const
{
   return( m_pAEG );
}


void Sample::setName( std::string name )
{
   m_Name = name;
}


bool Sample::getReverse() const
{
   return( m_Reverse );
}


void Sample::setReverse( bool r )
{
   m_Reverse = r;
}


void Sample::setPlayMode( Sample::PlayMode pm )
{
   m_PlayMode = pm;
}


Sample::PlayMode Sample::getPlayMode() const
{
   return( m_PlayMode );
}


std::string Sample::toString( Sample::PlayMode mode )
{
   switch( mode )
   {
      case PlayModeStandard:
         return( "Standard" );
         break;
      case PlayModeLoop:
         return( "Loop" );
         break;
      case PlayModeShot:
         return( "Shot" );
         break;
      case PlayModeLoopUntilRelease:
         return( "LoopUntilRelease" );
         break;
      default:
         return( "Standard" );
         break;
   }
}


Sample::PlayMode Sample::fromString( std::string mode )
{
   if( mode == "Standard" )
   {
      return( PlayModeStandard );
   } else
   if( mode == "Loop" )
   {
      return( PlayModeLoop );
   } else
   if( mode == "Shot" )
   {
      return( PlayModeShot );
   } else
   if( mode == "LoopUntilRelease" )
   {
      return( PlayModeLoopUntilRelease );
   } else
   {
      return( PlayModeStandard );
   }
}


bool Sample::matchesMidiNote( int note ) const
{
   return( ( m_MinNote <= note ) && ( note <= m_MaxNote ) );
}


bool Sample::matchesVelocity( int vel ) const
{
   return( ( m_MinVelocity <= vel ) && ( vel <= m_MaxVelocity ) );
}


std::set<Sample::PlayMode> Sample::allPlayModes()
{
   static std::set<Sample::PlayMode> m;

   if( m.size() == 0 )
   {
      m.insert( Sample::PlayModeStandard );
      m.insert( Sample::PlayModeLoop );
      m.insert( Sample::PlayModeShot );
      m.insert( Sample::PlayModeLoopUntilRelease );
   }

   return( m );
}


std::string Sample::getName() const
{
   return( m_Name );
}


WaveFile *Sample::getWave() const
{
   return( m_pWave );
}


int Sample::getBaseNote() const
{
   return( m_BaseNote );
}


void Sample::setBaseNote( int note )
{
   m_BaseNote = note;
}


int Sample::getMinNote() const
{
   return( m_MinNote );
}


int Sample::getMaxNote() const
{
   return( m_MaxNote );
}


void Sample::setMinNote( int note )
{
   m_MinNote = note;
}


void Sample::setMaxNote( int note )
{
   m_MaxNote = note;
}


int Sample::getMinVelocity() const
{
   return( m_MinVelocity );
}


void Sample::setMinVelocity( int v )
{
   m_MinVelocity = v;
}


int Sample::getMaxVelocity() const
{
   return( m_MaxVelocity );
}


void Sample::setMaxVelocity( int v )
{
   m_MaxVelocity = v;
}


void Sample::correctMinMaxNote()
{
   if( m_MinNote > m_MaxNote )
   {
      int tmp = m_MinNote;
      m_MinNote = m_MaxNote;
      m_MaxNote = tmp;
   }
}


float Sample::getDetune() const
{
   return( m_DetuneCents );
}


void Sample::setDetune( float d )
{
   m_DetuneCents = d;
}
