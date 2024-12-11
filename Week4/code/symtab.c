/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
enum
{
  false,
  true
};
void freeObject(Object *obj);
void freeScope(Scope *scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab *symtab;
Type *intType;
Type *charType;

/******************* Type utilities ******************************/

Type *makeIntType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type *makeCharType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type *makeArrayType(int arraySize, Type *elementType)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type *duplicateType(Type *type)
{
  // DONE
  Type *newType = (Type *)malloc(sizeof(Type));
  newType->typeClass = type->typeClass;
  newType->arraySize = type->arraySize;
  newType->elementType = type->elementType;
  return newType;
}

int compareType(Type *type1, Type *type2)
{
  // DONE
  if (type1->typeClass != type2->typeClass)
    return false;
  if (type1->typeClass == TP_ARRAY)
  {
    if (type1->arraySize != type2->arraySize)
      return false;
    return compareType(type1->elementType, type2->elementType);
  }
  return true;
}

void freeType(Type *type)
{
  // DONE
  if (type->typeClass == TP_ARRAY)
    freeType(type->elementType);
  free(type);
}

/******************* Constant utility ******************************/

ConstantValue *makeIntConstant(int i)
{
  // DONE
  ConstantValue *constant = (ConstantValue *)malloc(sizeof(ConstantValue));
  constant->type = TP_INT;
  constant->intValue = i;
  return constant;
}

ConstantValue *makeCharConstant(char ch)
{
  // DONE
  ConstantValue *constant = (ConstantValue *)malloc(sizeof(ConstantValue));
  constant->type = TP_CHAR;
  constant->charValue = ch;
  return constant;
}

ConstantValue *duplicateConstantValue(ConstantValue *v)
{
  // DONE
  ConstantValue *constant = (ConstantValue *)malloc(sizeof(ConstantValue));
  constant->type = v->type;
  constant->intValue = v->intValue;
  constant->charValue = v->charValue;
  return constant;
}

/******************* Object utilities ******************************/

Scope *createScope(Object *owner, Scope *outer)
{
  Scope *scope = (Scope *)malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object *createProgramObject(char *programName)
{
  Object *program = (Object *)malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes *)malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program, NULL);
  symtab->program = program;

  return program;
}

Object *createConstantObject(char *name)
{
  // DONE
  Object *constant = (Object *)malloc(sizeof(Object));
  strcpy(constant->name, name);
  constant->kind = OBJ_CONSTANT;
  constant->constAttrs = (ConstantAttributes *)malloc(sizeof(ConstantAttributes));
  return constant;
}

Object *createTypeObject(char *name)
{
  // DONE
  Object *type = (Object *)malloc(sizeof(Object));
  strcpy(type->name, name);
  type->kind = OBJ_TYPE;
  type->typeAttrs = (TypeAttributes *)malloc(sizeof(TypeAttributes));
  return type;
}

Object *createVariableObject(char *name)
{
  // DONE
  Object *variable = (Object *)malloc(sizeof(Object));
  strcpy(variable->name, name);
  variable->kind = OBJ_VARIABLE;
  variable->varAttrs = (VariableAttributes *)malloc(sizeof(VariableAttributes));
  return variable;
}

Object *createFunctionObject(char *name)
{
  // DONE
  Object *function = (Object *)malloc(sizeof(Object));
  strcpy(function->name, name);
  function->kind = OBJ_FUNCTION;
  function->funcAttrs = (FunctionAttributes *)malloc(sizeof(FunctionAttributes));
  function->funcAttrs->paramList = NULL;
  function->funcAttrs->returnType = NULL;
  function->funcAttrs->scope = createScope(function, symtab->currentScope);
  return function;
}

Object *createProcedureObject(char *name)
{
  // DONE
  Object *procedure = (Object *)malloc(sizeof(Object));
  strcpy(procedure->name, name);
  procedure->kind = OBJ_PROCEDURE;
  procedure->procAttrs = (ProcedureAttributes *)malloc(sizeof(ProcedureAttributes));
  procedure->procAttrs->paramList = NULL;
  procedure->procAttrs->scope = createScope(procedure, symtab->currentScope);
  return procedure;
}

Object *createParameterObject(char *name, enum ParamKind kind, Object *owner)
{
  // DONE
  Object *parameter = (Object *)malloc(sizeof(Object));
  strcpy(parameter->name, name);
  parameter->kind = OBJ_PARAMETER;
  parameter->paramAttrs = (ParameterAttributes *)malloc(sizeof(ParameterAttributes));
  parameter->paramAttrs->kind = kind;
  parameter->paramAttrs->function = owner;
  return parameter;
}

void freeObject(Object *obj)
{
  // DONE
  switch (obj->kind)
  {
  case OBJ_CONSTANT:
    free(obj->constAttrs);
    break;
  case OBJ_TYPE:
    free(obj->typeAttrs);
    break;
  case OBJ_VARIABLE:
    free(obj->varAttrs);
    break;
  case OBJ_FUNCTION:
    freeScope(obj->funcAttrs->scope);
    free(obj->funcAttrs);
    break;
  case OBJ_PROCEDURE:
    freeScope(obj->procAttrs->scope);
    free(obj->procAttrs);
    break;
  case OBJ_PARAMETER:
    free(obj->paramAttrs);
    break;
  default:
    break;
  }
}

void freeScope(Scope *scope)
{
  // DONE
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList)
{
  // DONE
  ObjectNode *node = objList;
  while (node != NULL)
  {
    ObjectNode *temp = node;
    node = node->next;
    freeObject(temp->object);
    free(temp);
  }
}

void freeReferenceList(ObjectNode *objList)
{
  // DONE
  ObjectNode *node = objList;
  while (node != NULL)
  {
    ObjectNode *temp = node;
    node = node->next;
    free(temp);
  }
}

void addObject(ObjectNode **objList, Object *obj)
{
  ObjectNode *node = (ObjectNode *)malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL)
    *objList = node;
  else
  {
    ObjectNode *n = *objList;
    while (n->next != NULL)
      n = n->next;
    n->next = node;
  }
}

Object *findObject(ObjectNode *objList, char *name)
{
  // DONE
  ObjectNode *node = objList;
  while (node != NULL)
  {
    if (strcmp(node->object->name, name) == 0)
      return node->object;
    node = node->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void)
{
  Object *obj;
  Object *param;

  symtab = (SymTab *)malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;

  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void)
{
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope *scope)
{
  symtab->currentScope = scope;
}

void exitBlock(void)
{
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object *obj)
{
  if (obj->kind == OBJ_PARAMETER)
  {
    Object *owner = symtab->currentScope->owner;
    switch (owner->kind)
    {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }

  addObject(&(symtab->currentScope->objList), obj);
}
