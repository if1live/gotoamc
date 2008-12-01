#include "frame.h"
#include "textFrame.h"
#include "constNum.h"

/*class TextFrame2PPM
{
public :
    TextFrame2PPM();
    void Convert( TextFrame* _inputFrame );


    ~TextFrame2PPM();
  
private :
    Frame* fonts[MAX_OF_FONTS];

    };*/

class TextFrame2PPM
{
 public :
  TextFrame2PPM();
  void Convert();


  ~TextFrame2PPM();

 private :
  Frame *fonts[MAX_OF_FONTS];

};
