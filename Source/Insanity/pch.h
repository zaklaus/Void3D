#include <windows.h>
#include <rules.h>
#include <memory.h>
#include <stdio.h>
#include <C_vector.h>

#pragma warning(push,3)
#include <map>
#pragma warning(pop)

#include <win_reg.h>
#include <SmartPtr.h>
#include <c_unknwn.hpp>
#include <C_str.hpp>
#include <c_cache.h>
#include <dta_read.h>
#include <igraph2.h>
#include <i3d\i3d2.h>

#include <insanity\os.h>
#include <insanity\SourceControl.h>
#include <insanity\sprite.h>
#include <insanity\3dtexts.h>
#include <insanity\controller.h>
#include <insanity\texts.h>

using namespace std;

inline int FloatToInt(float f){
   return (int)f;
}
