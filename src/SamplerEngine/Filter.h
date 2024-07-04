/*----------------------------------------------------------------------------*/
/*!
\file Filter.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for class Filter.
*/
/*----------------------------------------------------------------------------*/
#ifndef __FILTER_H__
#define __FILTER_H__

#include <set>
#include <libxml/tree.h>

//==============================================================================
namespace SamplerEngine
{
   /*----------------------------------------------------------------------------*/
   /*!
   \class Filter
   \date  2024-06-10
   */
   /*----------------------------------------------------------------------------*/
   class Filter
   {
   public:
      enum Type
      {
         TYPE_NONE = 1,
         TYPE_LOWPASS,
         TYPE_HIGHPASS
      };

      Filter();
      Filter( const Filter &d );
      ~Filter();

      void getSettings( const Filter &d );

      void setCutoff( double v );
      void setResonance( double v );
      void setType( Type type );

      void setCutoffMod( double v );
      double getCutoffMod() const;

      void setResonanceMod( double v );
      double getResonanceMod() const;

      double getCutoff() const;
      double getResonance() const;
      Type getType() const;

      void process( float *pLSamples, float *pRSamples, const uint32_t n, double sampleRate );
      static void process( float *pLSamples, float *pRSamples, const uint32_t n, double sampleRate, Type type, double cutoff, double resonance, double *pXL, double *pXR, double *pYL, double *pYR );

      static std::string toString( Type type );
      static Type fromString( const std::string &str );
      static std::set<Type> allTypes();

      static Filter *fromXml( xmlNode *pe );
      xmlNode *toXml() const;

   protected:

   private:
      Type m_Type;
      double m_CutoffMod;
      double m_Cutoff;
      double m_Resonance;
      double m_ResonanceMod;
      double m_X[2][2];
      double m_Y[2][2];
   };
}
#endif

