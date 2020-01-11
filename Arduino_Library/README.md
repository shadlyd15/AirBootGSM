
# PrettyDebug
A lightweight debug library written in **C/C++**. 
It is designed especially for **embedded systems** keeping in mind the limitation and capabilities of the platform.
  - Low memory usage
  - Supports a wide range of compilers and processor architectures
  - Arduino compatible
  - ESP32 & ESP8266 supported
  - Color debug output
  - Very easy to use
  
## How to use

- **Attach Debug Stream :**
```C
	ATTACH_DEBUG_STREAM(&Serial);
```	

- **Print Green OK Message :**
```C
	DEBUG_OK("An Example OK Message From %s, "PrettyDebug");
```

- **Print Red ERROR Message :**
```C
	DEBUG_ERROR("An Example ERROR Message From %s, "PrettyDebug");
```

- **Print Red ERROR Message :**
```C
	DEBUG_ERROR("An Example ERROR Message From %s, "PrettyDebug");
```

- **Print Cyan ALERT Message :**
```C
	DEBUG_ALERT("An Example ALERT Message From %s, "PrettyDebug");
```

- **Print Yellow WARNING Message :**
```C
	DEBUG_WARNING("An Example WARNING Message From %s, "PrettyDebug");
```

- **Print Variable with Variable Name :**
```C
	DEBUG_VARIABLE("%d", Sample_Value);
```	

- **Print Array with Name :**
```C
	DEBUG_ARRAY(Sample_Array, 16, "%02X");
```	

**Print Current Location in Code :**
```C
	DEBUG_TRACE();
```	

## Arduino Example 

``` C
	#include "PrettyDebug.h"

	int Sample_Variable = 123;
	int Sample_Array[] = {1, 2, 3, 4, 5};

	void setup(){
	    Serial.begin(115200);
	    ATTACH_DEBUG_STREAM(&Serial);

	    DEBUG_OK("Pretty Debug Example Sketch");
	    DEBUG_TRACE();

	    DEBUG_OK("An Example OK Message From %s, "PrettyDebug");
	    DEBUG_ERROR("An Example ERROR Message From %s, "PrettyDebug");
	    DEBUG_ERROR("An Example ERROR Message From %s, "PrettyDebug");
	    DEBUG_ALERT("An Example ALERT Message From %s, "PrettyDebug");
	    DEBUG_WARNING("An Example WARNING Message From %s, "PrettyDebug");
	    DEBUG_VALUE(Sample_Value, "%d");
	    DEBUG_ARRAY(Sample_Array, 16, "%02X");
	}

	void loop(){

	}
 ```

## Sample Output

<div style="text-align:center"><img src ="https://raw.githubusercontent.com/shadlyd15/prettydebug/master/images/output.png" alt ="Sample Output"/></div>