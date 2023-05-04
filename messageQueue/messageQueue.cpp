#include "messageQueue.h"
#include <python2.7/Python.h>

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define BUFFER_SIZE 1024
int callPython(const char *src,const char *func, int arg, ...){
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    pName = PyString_FromString(src); //get python src

    /*Error checking of pName*/
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, func);

        if (pFunc && PyCallable_Check(pFunc)) {

            va_list ap;
            va_start(ap,arg);
            pArgs = PyTuple_New(arg);
            for (i = 0; i < arg; ++i) {
                pValue = PyInt_FromLong(va_arg(ap,int));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs); // call function with argument
            Py_DECREF(pArgs);
            /*value checking*/
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            /*No return value*/
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
            va_end(ap);
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", func);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", src);
        return 1;
    }
    Py_Finalize();
    return PyInt_AsLong(pValue);
    
}

using namespace std;

struct MsgBuf {
    long msgtype; // message type, must be > 0
    int value;
    char buf[BUFFER_SIZE];
};


void * producer(void *arg){
    int cnt = 0;
    int key_id;
    MsgBuf msg;
    msg.msgtype = 1;

    key_id = msgget((key_t) 4567, IPC_CREAT|0666);

    if (key_id == -1) {
        cerr << "Message Get Failed!" << endl;
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        msg.value = ++cnt;
    
        if (cnt >= 10) {
            cout << "Message Sending Finished!" << endl;
            break;
        }

        strcpy(msg.buf, "Hello from C++ producer!");
        if (msgsnd(key_id, &msg, sizeof(msg), IPC_NOWAIT) == -1) {
            cerr << "Message Sending Failed!" << endl;
            exit(EXIT_FAILURE);
        }
        
        cout << "value: " << msg.value << endl;
        sleep(1);
    }
    exit(EXIT_SUCCESS);
    return 0;
}

void * consumer(void *arg){
    const char *src = "consumer";
    const char *func = "consumer";
    callPython(src, func, 1, 1);   
    return 0;
}





