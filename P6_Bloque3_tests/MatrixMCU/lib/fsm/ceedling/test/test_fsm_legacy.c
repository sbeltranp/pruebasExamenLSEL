#include "unity.h"

#define TEST_CASE(...)
#define TEST_RANGE(...)

#include "fsm.h"
#include "mock_test_fsm.h"

#include <stdlib.h>

/**
 * @file test_fsm_legacy.c
 * @author 
 * @author 
 * @brief Tests for existing fsm module
 * @version 0.1
 * @date 2024-04-09
 * 
 */

/**
 * @brief Stub or Callback for fsm_malloc that calls real malloc. 
 * 
 * @param[in] s Amount of bytes to allocate
 * @param[in] n Amount of calls to this function
 * 
 * @return pointer to allocated memory if success; NULL if fails
 * 
 */
static void* cb_malloc(size_t s, int n) {
    return malloc(s);
}

/**
 * @brief Stub or Callback for fsm_free that calls real free. 
 * 
 * @param[in] p Pointer to allocated memory to free
 * @param[in] n Amount of calls to this function
 * 
 */
static void cb_free(void* p, int n) {
    return free(p);
}

void setUp(void)
{
}

void tearDown(void)
{
}

/**
 * @brief Comprueba que la funcion de fsm_new devuelve NULL 
 *        y no llama a fsm_malloc si la tabla de transiciones es NULL 
 */
void test_fsm_new_nullWhenNullTransition(void)
{
    fsm_t *f = (fsm_t*)1;

    f = fsm_new(NULL);

    TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief Comprueba que la función de inicialización devuelve false si el puntero a la maquina 
 * de estado es NULL 
 *
 */
void test_fsm_init_falseWhenNullFsm(void)
{
    /*IGNORAR HASTA QUE LO ARREGLE PEDRO*/
    // setup
    bool result;
    fsm_t *f = NULL;
    fsm_trans_t tt[] = {{0, is_true, 1, do_nothing}};
    
    // code under test
    result = fsm_init(f,tt); 

    // test
    TEST_ASSERT_EQUAL(false, result);
}

/**
 * @brief Función de inicializacion devuelve false si la tabla de transiciones es nula
 * 
 */
void test_fsm_init_falseWhenNullTransitions(void)
{
    bool result;
    //fsm_t *f = (fsm_t*)1;
    fsm_t f;
    
    // code under test
    result = fsm_init(&f, NULL); 

    // test
    TEST_ASSERT_EQUAL(false, result);
    
}

/**
* @brief La máquina de estados devuelve NULL 
*        y no llama a fsm_malloc si el estado de origen 
*        de la primera transición es -1 (fin de la tabla)
*/
void test_fsm_nullWhenFirstOrigStateIsMinusOne (void) {
  fsm_trans_t tt[] = {{-1, is_true, 1, do_nothing}};
  fsm_t *f = (fsm_t*)1;
  f = fsm_new(tt);
 
    TEST_ASSERT_EQUAL (NULL, f);
//   TEST_FAIL_MESSAGE("Implement the test");
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino
 *  de la primera transición es -1 (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstDstStateIsMinusOne (void) {
  fsm_trans_t tt[] = {{0, is_true, -1, do_nothing}};
  fsm_t *f = (fsm_t*)1;
  f = fsm_new(tt);
 
  TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsmfsm_init_malloc si la función 
 * de comprobación de la primera transición es NULL (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstCheckFunctionIsNull (void) {
   TEST_IGNORE(); // Las nuevas funciones hacen que no sea necesario
  
  fsm_trans_t tt[] = {{0, NULL, 1, do_nothing}};
  fsm_t *f = (fsm_t*)1;

  f = fsm_new(tt);
 
  //TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief Devuelve puntero no NULL y llama a fsm_malloc (Stub) al crear la maquina de estados con 
 *        una transición válida con función de actualización (salida) NULL o no NULL.
 *        Hay que liberar la memoria al final llamando a free
 * 
 */
TEST_CASE(NULL)
TEST_CASE(do_nothing)
void test_fsm_new_nonNullWhenOneValidTransitionCondition(fsm_output_func_t out)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, out},
        {-1, NULL, -1, NULL}
    };
    
    fsm_malloc_Stub(cb_malloc);
    //fsm_free_Stub(cb_free);

    fsm_t *f = (fsm_t*)1;
    f = fsm_new(tt);
    
    TEST_ASSERT_NOT_NULL(f);

    cb_free(f, 1); // Liberar memoria //free(f);

}


/**
 * @brief Estado inicial corresponde al estado de entrada de la primera transición
 *  de la lista al crear una maquina de estados y es valido. 
 * 
 */
void test_fsm_new_fsmGetStateReturnsOrigStateOfFirstTransitionAfterInit(void)
{
    int orig_state;
    fsm_trans_t tt[] = {
        {17, is_true, 1, do_nothing},
        {-1, NULL, -1, NULL}
    };

    fsm_t *f = (fsm_t*)1;

    fsm_malloc_Stub(cb_malloc);
    fsm_free_Stub(cb_free);

    f = fsm_new(tt);
    orig_state = fsm_get_state(f);
    
    TEST_ASSERT_EQUAL(17, orig_state);
}

/**
 * @brief La maquina de estado no transiciona si la funcion devuelve 0
 * 
 */
void test_fsm_fire_isTrueReturnsFalseMeansDoNothingIsNotCalledAndStateKeepsTheSame(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing},
        {-1, NULL, -1, NULL}
    };

    fsm_t f;
    int res;

    //is_true_ExpectAnyArgsAndReturn(0);
    is_true_IgnoreAndReturn(0);

    fsm_init(&f, tt);
    fsm_fire(&f);
    res = fsm_get_state(&f);

   TEST_ASSERT_EQUAL(0, res);
   //TEST_IGNORE();
}

/**
 * @brief Comprueba que el puntero pasado a fsm_fire es pasado a la 
 * función de guarda cuando se comprueba una transición
 * 
 */
void test_fsm_fire_checkFunctionCalledWithFsmPointerFromFsmFire(void)
{
    
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };

    fsm_t f ;
    
    fsm_malloc_Stub(cb_malloc);
    fsm_init(&f, tt);
    is_true_ExpectAndReturn(&f, 1);
    fsm_fire(&f);
    //TEST_IGNORE();
}

/** 
 * @brief Comprueba que el fsm_fire funciona y tiene el estado correcto cuando la 
 * transición devuelve true (cambia) y cuando devuelve false (mantiene)
 * 
 */
TEST_CASE(false, 0)
TEST_CASE(true, 1)
void test_fsm_fire_checkFunctionIsCalledAndResultIsImportantForTransition(bool returnValue, int expectedState)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };

    fsm_t f;
    fsm_init(&f, tt);
    
    fsm_malloc_Stub(cb_malloc);
    
    is_true_ExpectAnyArgsAndReturn(returnValue);

    fsm_fire(&f);

    TEST_ASSERT_EQUAL(expectedState, fsm_get_state(&f));

}


/**
 * @brief La creación de una máquina de estados devuelve NULL si la reserva de memoria falla (Mock, no Stub)
 * 
 */
void test_fsm_new_nullWhenFsmMallocReturnsNull(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t *f;
    fsm_malloc_ExpectAndReturn(sizeof(fsm_t), NULL);
    f = fsm_new(tt);
    
    //TEST_IGNORE();
    TEST_ASSERT_EQUAL(NULL, f);
}

/**
 * @brief Llamar a fsm_destroy provoca una llamada a fsm_free (Mock, no Stub)
 * 
 */
void test_fsm_destroy_callsFsmFree(void)
{

    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t *f;
    fsm_malloc_Stub(cb_malloc);
    f = fsm_new(tt);
    fsm_free_ExpectAnyArgs();
    fsm_destroy(f);
}

/**
 * @brief Comprueba que solo se llame a la función de guarda que toca según el estado actual
 * 
 */
void test_fsm_fire_callsFirstIsTrueFromState0AndThenIsTrue2FromState1(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true2, 0, NULL},   //Descomentar cuando se haya declarado una nueva función para mock is_true2
        {-1, NULL, -1, NULL}
    };

    fsm_t f;
    int res;
    fsm_init(&f, tt);
    res = fsm_get_state(&f);
    if (res == 0)
    {
        is_true_ExpectAnyArgsAndReturn(1);
    }
    fsm_fire(&f);
    if (res == 1)
    {
        is_true2_ExpectAnyArgsAndReturn(1);
    }

    //TEST_IGNORE();
}

/**
 * @brief Comprueba que se pueden crear dos instancias de máquinas de estados simultánteas 
 * y son punteros distintos
 * 
 */
void test_fsm_new_calledTwiceWithSameValidDataCreatesDifferentInstancePointer(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };

    fsm_malloc_Stub(cb_malloc);

    fsm_t* f;
    f = fsm_new(tt);

    fsm_t* g;
    g = fsm_new(tt);

    TEST_ASSERT_NOT_EQUAL(f, g); 
    
}


/************************************************
 * **********************************************
*/
/* TEST NUEVOS */


/**
 * @brief fsm_init devuelve int con el número de transiciones
 * válidas (con máximo de 128 indicado en un #define
 * FSM_MAX_TRANSITIONS). Si es mayor, devuelve 0
 *
 */
void test_fsm_init_returnsIntNumberValidTransitions(void)
{
    int validTransitions = 0;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t f;
    validTransitions = fsm_init(&f, tt);
    TEST_ASSERT_EQUAL(2, validTransitions);

}

void test_fsm_init_returnsZeroWhenMoreThanMaxTransitions(void){

    int validTransitions;
    int i;
    fsm_trans_t tt[FSM_MAX_TRANSITIONS +2];
    for (i = 0; i < FSM_MAX_TRANSITIONS +1; i++) {
        tt[i].orig_state = i;
        tt[i].in = is_true;
        tt[i].dest_state = i + 1;
        tt[i].out = NULL;
    }
    tt[i].orig_state = -1;
    tt[i].dest_state = -1;
    tt[i].in = NULL;
    tt[i].out = NULL;
    fsm_t f;
    validTransitions = fsm_init(&f, tt);
    TEST_ASSERT_EQUAL(0, validTransitions);

}


/**
 * @brief Una función de guarda NULL en una transición equivale
 * a que se cumple siempre (equivale a devuelve true)
 * 
 */
void test_fsm_init_nullCheckFunctionMeansAlwaysTrue(void)
{
    int validTransitions = 0;
    
    fsm_trans_t tt[] = {
        {0, NULL, 1, NULL},
        {1, is_true, 0, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t f;
    validTransitions = fsm_init(&f, tt);
    
    TEST_ASSERT_EQUAL(2, validTransitions);

}
/** 
 * @brief Añadir valor de retorno int a fsm_fire:
 * -1: si no hay ninguna transición para el estado actual
 * 0: si hay transiciones para el estado actual pero la función de
 * guarda devuelve false
 * 1: si hay al menos una transición para el estado actual con la
 * función de guarda true
 * 
 */
 void test_fsm_fire_ReturnIntMinusOneNoTransitionForCurrentState(void)
 {
    int result = 0;
    fsm_trans_t tt[] = {
        {0, is_true, 2, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t* f;
    
    fsm_malloc_Stub(cb_malloc);
    is_true_IgnoreAndReturn(1);


    f = fsm_new(tt);
    result = fsm_fire(f);
    result = fsm_fire(f);

    TEST_ASSERT_EQUAL(-1, result);
 }

/** 
 * @brief Añadir valor de retorno int a fsm_fire:
 * -1: si no hay ninguna transición para el estado actual
 * 0: si hay transiciones para el estado actual pero la función de
 * guarda devuelve false
 * 1: si hay al menos una transición para el estado actual con la
 * función de guarda true
 * 
 */
 void test_fsm_fire_ReturnIntOneIfTransitionForCurrentStateAndTrue(void)
 {
    int result = 0;
    fsm_trans_t tt[] = {
        {0, is_true, 2, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t* f;
    
    fsm_malloc_Stub(cb_malloc);
    // fsm_malloc_Stub(cb_malloc);
    is_true_ExpectAnyArgsAndReturn(1);
    //is_true_ExpectAnyArgsAndReturn(1);

    f = fsm_new(tt);
    result = fsm_fire(f);

    TEST_ASSERT_EQUAL(1, result);
 }

/** 
 * @brief Añadir valor de retorno int a fsm_fire:
 * -1: si no hay ninguna transición para el estado actual
 * 0: si hay transiciones para el estado actual pero la función de
 * guarda devuelve false
 * 1: si hay al menos una transición para el estado actual con la
 * función de guarda true
 * 
 */
void test_fsm_fire_ReturnIntZeroIfTransitionForCurrentStateAndFalse(void)
 {
    int result = 0;
    fsm_trans_t tt[] = {
        {0, is_true, 2, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t* f;
    
    fsm_malloc_Stub(cb_malloc);
    is_true_IgnoreAndReturn(0);

    f = fsm_new(tt);
    result = fsm_fire(f);

    TEST_ASSERT_EQUAL(0, result);
 }



/**
 * @brief ) Llamada a fsm_destroy llama a fsm_free solo si el
 * puntero pasado no es NULL
 * 
 */
void test_fsm_destroy_NotCallsFsmFreeOnlyIfPointerIsNull(void) {
    fsm_t f;
    fsm_malloc_Stub(cb_malloc);
    fsm_free_Stub(cb_free);
    //fsm_init(&f, NULL);
    //fsm_free_ExpectAnyArgs();
    fsm_destroy(NULL);
}

/**
 * @brief ) Llamada a fsm_destroy llama a fsm_free solo si el
 * puntero pasado no es NULL
 * 
 */
void test_fsm_destroy_callsFsmFreeOnlyIfPointerIsNotNull(void) {
    int validTrans;
    fsm_t *f;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_malloc_Stub(cb_malloc);
    fsm_free_Stub(cb_free);
    f = fsm_new(tt);
    fsm_free_ExpectAnyArgs();
    fsm_destroy(f);
    //fsm_free_ExpectAnyArgs();
}

