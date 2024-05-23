#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <data_set.h>


static void free_PyObject(PyObject* obj) { 
    free(PyCapsule_GetPointer(obj, "DataSet"));
}


static DataSet* PyObject_toDataSet(PyObject* obj) { 
    return (DataSet*)PyCapsule_GetPointer(obj, "DataSet");
}


static PyObject* DataSet_toPyObject(DataSet* dataSet, int must_free) { 
    return PyCapsule_New(dataSet, "DataSet", free_PyObject);
}


static PyObject* test_DataSet_allocate(PyObject* self, PyObject* args) {
    const char* fileName;
    int length, batchSize, inputLength, outputLength;

    if (!PyArg_ParseTuple(args, "siiii", &fileName, &length, &batchSize, &inputLength, &outputLength)) {
        #ifdef DEBUG
        printf("!!! ERROR in test_DataSet_allocate() in PyArg_ParseTuple() !!!\n");
        #endif
        return NULL;
    }

    DataSet* dataSet;
    DataSet_allocate(dataSet, fileName, length, batchSize, inputLength, outputLength);

    #ifdef DEBUG
    printf("Construct\n");
    #endif
    return DataSet_toPyObject(dataSet, 1);
}


static PyObject* test_DataSet_deallocate(PyObject* self, PyObject* args) {
    PyObject* py_dataSet = NULL;
    printf("asdasdas\n");
    if (!PyArg_ParseTuple(args, "O", &py_dataSet)) {
        #ifdef DEBUG
        printf("!!! ERROR in test_DataSet_deallocate() in PyArg_ParseTuple() !!!\n");
        #endif
        return NULL;
    }
    
    DataSet* dataSet = PyObject_toDataSet(py_dataSet);
    DataSet_deallocate(dataSet);
    
    #ifdef DEBUG
    printf("Destruct\n");
    #endif
    Py_RETURN_NONE;
}


static PyMethodDef test_methods[] = {
    {"DataSet_allocate", test_DataSet_allocate, METH_VARARGS, ""},
    {"DataSet_deallocate", test_DataSet_deallocate, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef test_module = {
    PyModuleDef_HEAD_INIT,
    "test",
    NULL,
    -1,
    test_methods
};


PyMODINIT_FUNC PyInit_test(void) {
    return PyModule_Create(&test_module);
}
