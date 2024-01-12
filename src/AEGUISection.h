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
   virtual ENV *getENV( const Sample *pSample ) const;
};

#endif
