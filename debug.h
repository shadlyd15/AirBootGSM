#ifndef DEBUG_H
#define DEBUG_H
	#ifdef ENABLE_DEBUG
		#define DEBUG(x) x
	#else
		#define DEBUG(x)
	#endif
#endif