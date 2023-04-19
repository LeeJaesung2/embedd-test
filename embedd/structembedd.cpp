#include <python2.7/Python.h>

struct carData{
    int road_id;
    int waypoint_id;
    float velocity;
};

struct droneData{
    int road_id;
    int waypoint_id;
    float velocity;
    float will_go_waypoint[100];
};



int
main(int argc, char *argv[])
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue, *pDict;
    carData car_data = {1, 55, 87.4};
    droneData drone_data = {5, 8, 60.8, {122.2, 8.33}};
    const char* msg = "hello world";


    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    pName = PyString_FromString("structtest");
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "my_function");
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(2);
            pValue = Py_BuildValue("s", msg);
            PyTuple_SetItem(pArgs, 0, pValue);
            pValue = Py_BuildValue("iif", car_data.road_id, car_data.waypoint_id, car_data.velocity);
            PyTuple_SetItem(pArgs, 1, pValue);
            // pValue = Py_BuildValue("icf", point2.x, point2.y, point2.z);
            // PyTuple_SetItem(pArgs, 1, pValue);
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                pDict = PyDict_Copy(pValue);
                Py_DECREF(pValue);

                // Extract values from dictionary
                PyObject *pRoadID = PyDict_GetItemString(pDict, "road_id");
                int road_id = PyLong_AsLong(pRoadID);

                PyObject *pWaypointID = PyDict_GetItemString(pDict, "waypoint_id");
                int waypoint_id = PyLong_AsLong(pWaypointID);

                PyObject *pVelocity = PyDict_GetItemString(pDict, "velocity");
                float velocity = PyFloat_AsDouble(pVelocity);

                printf("Road ID: %d, Waypoint ID: %d, Velocity: %.2f\n", road_id, waypoint_id, velocity);
                Py_DECREF(pDict);
            }

            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", "my_function");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", "my_module");
        return 1;
    }
    Py_Finalize();
    return 0;
}
