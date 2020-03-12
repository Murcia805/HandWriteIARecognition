//
// Created by Guaberx on 9/21/2016.
//

#ifndef HANDWRITERECOGNITION_MACROS_H
#define HANDWRITERECOGNITION_MACROS_H

/*Esto se va a descontrolar!!!!*/
#define min(x,y) ((x) < (y) ? (x):(y));
#define PROPRINT(x) cout << x << endl;
#define PRO1 1
#define PRO2 2

#define PRO(x) PRO##x

#define ABRIRARCH(filename,path,arguments) fstream filename;\
                            filename.open(path,arguments);

#define MAIN(x) x()

#endif //HANDWRITERECOGNITION_MACROS_H
