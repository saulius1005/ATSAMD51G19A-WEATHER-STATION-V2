#include "settings.h"
#include "SensorsVar.h"


char * WindDirNames(){ // return wind direction short name
	switch(sensors.WIND.direction) {        
		case 1: return "NE";  // Northeast
		case 2: return "E ";  // East
		case 3: return "SE"; // Southeast
		case 4: return "S ";  // South
		case 5: return "SW"; // Southwest
		case 6: return "W ";  // West
		case 7: return "NW"; // Northwest
	}
	return "N ";  // North
}
