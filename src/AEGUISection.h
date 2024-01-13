#ifndef __AEGUISECTION_H__
#define __AEGUISECTION_H__

#include "JuceHeader.h"

#include "EGUISection.h"

class PluginEditor;

//==============================================================================
class AEGUISection : public EGUISection
{
public:
   AEGUISection( PluginEditor *pEditor );
   ~AEGUISection();

protected:
   virtual Overvoltage::ENV *getENV( const Overvoltage::Sample *pSample ) const;
};

#endif
