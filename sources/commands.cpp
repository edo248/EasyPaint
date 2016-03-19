
#include "commands.h"
#include "instruments/lineinstrument.h"

void draw_line(ImageArea &ia, QPoint p1, QPoint p2, bool primaryColor)
{

    LineInstrument *instrumentHandler;

    instrumentHandler = qobject_cast<LineInstrument*>(ia.getInstrument(LINE));

    instrumentHandler->initInstrumentAction(ia);
    instrumentHandler->setStartPoint(p1);
    instrumentHandler->setEndPoint(p2);
    instrumentHandler->setIsSecondaryColor(!primaryColor);
    instrumentHandler->completeInstrumentAction(ia);

}
