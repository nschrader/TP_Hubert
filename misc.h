#ifndef MISC_H
#define MISC_H

#define fatal(x) _fatal("[Fatal] " x);
void _fatal(char* errorMessage);

#define warning(x) _warning("[Warning] " x);
void _warning(char* errorMessage);

#endif
