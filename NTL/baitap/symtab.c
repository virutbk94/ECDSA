/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void)
{
    Type* type = (Type*) malloc(sizeof(Type));
    type->typeClass = TP_INT;
    return type;
}

Type* makeCharType(void)
{
    Type* type = (Type*) malloc(sizeof(Type));
    type->typeClass = TP_CHAR;
    return type;
}

Type* makeArrayType(int arraySize, Type* elementType)
{
    Type* type = (Type*) malloc(sizeof(Type));
    type->typeClass = TP_ARRAY;
    type->arraySize = arraySize;
    type->elementType = elementType;
    return type;
}

Type* duplicateType(Type* type)
{
    // TODO
    Type* typeDuplicate = (Type*) malloc(sizeof(Type));
    typeDuplicate->typeClass = type->typeClass;
    if(typeDuplicate->typeClass == TP_ARRAY)
    {
        typeDuplicate->arraySize = type->arraySize;
        typeDuplicate->elementType = type->elementType;
    }
    return typeDuplicate;
}

int compareType(Type* type1, Type* type2)
{
    // TODO
    if(type1->typeClass == type2->typeClass)
    {
        if(type1->typeClass == TP_ARRAY)
        {
            if(type1->arraySize==type2->arraySize && compareType(type1->elementType,type2->elementType))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

void freeType(Type* type)
{
    // TODO
    switch (type->typeClass)
    {
    case TP_INT:
    case TP_CHAR:
        free(type);
        break;
    case TP_ARRAY:
        freeType(type->elementType);
        freeType(type);
        break;
    }
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i)
{
    // TODO
    ConstantValue* constantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
    constantValue->type = TP_INT;
    constantValue->intValue = i;
    return constantValue;
}

ConstantValue* makeCharConstant(char ch)
{
    // TODO
    ConstantValue* constantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
    constantValue->type = TP_CHAR;
    constantValue->charValue = ch;
    return constantValue;
}

ConstantValue* duplicateConstantValue(ConstantValue* v)
{
    // TODO
    ConstantValue* constantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
    constantValue->type = v->type;
    if(v->type == TP_INT)
    {
        constantValue->intValue = v->intValue;
    }
    else
    {
        constantValue->charValue=v->charValue ;
    }
    return constantValue;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer)
{
    Scope* scope = (Scope*) malloc(sizeof(Scope));
    scope->objList = NULL;
    scope->owner = owner;
    scope->outer = outer;
    return scope;
}

Object* createProgramObject(char *programName)
{
    Object* program = (Object*) malloc(sizeof(Object));
    strcpy(program->name, programName);
    program->kind = OBJ_PROGRAM;
    program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
    program->progAttrs->scope = createScope(program,NULL);
    symtab->program = program;

    return program;
}

Object* createConstantObject(char *name)
{
    // TODO
    Object* constantObject = (Object*) malloc(sizeof(Object));
    strcpy(constantObject->name,name);
    constantObject->kind = OBJ_CONSTANT;
    constantObject->constAttrs = (ConstantAttributes*) malloc(sizeof(ConstantAttributes));
    constantObject->constAttrs->value = NULL;
    return constantObject;
}

Object* createTypeObject(char *name)
{
    // TODO
    Object* typeObject = (Object*) malloc(sizeof(Object));
    strcpy(typeObject->name,name);
    typeObject->kind = OBJ_TYPE;
    typeObject->typeAttrs = (TypeAttributes*) malloc(sizeof(TypeAttributes));
    typeObject->typeAttrs->actualType = NULL;
    return typeObject;
}

Object* createVariableObject(char *name)
{
    // TODO
    Object* varObject = (Object*) malloc(sizeof(Object));
    strcpy(varObject->name,name);
    varObject->kind = OBJ_VARIABLE;
    varObject->varAttrs = (VariableAttributes*) malloc(sizeof(VariableAttributes));
    varObject->varAttrs->type = NULL;
    varObject->varAttrs->scope = NULL;
    return varObject;
}

Object* createFunctionObject(char *name)
{
    // TODO
    Object* funcObject = (Object*) malloc(sizeof(Object));
    strcpy(funcObject->name,name);
    funcObject->kind = OBJ_FUNCTION;
    funcObject->funcAttrs = (FunctionAttributes*) malloc(sizeof(FunctionAttributes));
    funcObject->funcAttrs->paramList = NULL;
    funcObject->funcAttrs->returnType = NULL;
    funcObject->funcAttrs->scope = createScope(funcObject,symtab->currentScope);
    return funcObject;
}

Object* createProcedureObject(char *name)
{
    // TODO
    Object* procObject = (Object*) malloc(sizeof(Object));
    strcpy(procObject->name,name);
    procObject->kind = OBJ_PROCEDURE;
    procObject->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));
    procObject->procAttrs->paramList = NULL;
    procObject->procAttrs->scope = createScope(procObject,symtab->currentScope);
    return procObject;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner)
{
    // TODO
    Object* paramObject = (Object*) malloc(sizeof(Object));
    strcpy(paramObject->name,name);
    paramObject->kind = OBJ_PARAMETER;
    paramObject->paramAttrs = (ParameterAttributes*) malloc(sizeof(ParameterAttributes));
    paramObject->paramAttrs->kind = kind;
    paramObject->paramAttrs->type = NULL;
    paramObject->paramAttrs->function = owner;
    return paramObject;
}

void freeObject(Object* obj)
{
    // TODO
    if(obj != NULL)
    {

        switch(obj->kind)
        {
        case OBJ_CONSTANT:
            if(obj->constAttrs != NULL)
            {
                if(obj->constAttrs->value != NULL)
                {
                    free(obj->constAttrs->value);
                    obj->constAttrs->value=NULL;
                }
                free(obj->constAttrs);
                obj->constAttrs=NULL;
            }
            break;
        case OBJ_PROGRAM:
            if(obj->progAttrs != NULL)
            {
                if(obj->progAttrs->scope != NULL)
                {
                    free(obj->progAttrs->scope);
                    obj->progAttrs->scope = NULL;
                }
                free(obj->progAttrs);
                obj->progAttrs = NULL;
            }
            break;
        case OBJ_VARIABLE:
            if(obj->varAttrs != NULL)
            {
                if(obj->varAttrs->type != NULL)
                {
                    free(obj->varAttrs->type);
                    obj->varAttrs->type = NULL;
                }
                free(obj->varAttrs);
                obj->varAttrs = NULL;
            }
            break;
        case OBJ_TYPE:
            if(obj->typeAttrs != NULL)
            {
                if(obj->typeAttrs->actualType != NULL)
                {
                    free(obj->typeAttrs->actualType);
                    obj->typeAttrs->actualType = NULL;
                }
                free(obj->typeAttrs);
                obj->typeAttrs = NULL;
            }
            break;
        case OBJ_FUNCTION:
            if(obj->funcAttrs != NULL)
            {
                if(obj->funcAttrs->scope != NULL)
                {
                    freeScope(obj->funcAttrs->scope);
                    obj->funcAttrs->scope = NULL;
                }
                free(obj->funcAttrs);
                obj->funcAttrs = NULL;
            }
            break;
        case OBJ_PROCEDURE:
            if(obj->procAttrs != NULL)
            {
                if(obj->procAttrs->scope != NULL)
                {
                    freeScope(obj->procAttrs->scope);
                    obj->procAttrs->scope = NULL;
                }
                free(obj->procAttrs);
                obj->procAttrs = NULL;
            }
            break;
        case OBJ_PARAMETER:
            if(obj->paramAttrs != NULL)
            {
                if(obj->paramAttrs->type != NULL)
                {
                    free(obj->paramAttrs->type);
                    obj->paramAttrs->type = NULL;
                }
                free(obj->paramAttrs);
                obj->paramAttrs = NULL;
            }
            break;
        }

        free(obj);
        obj = NULL;
    }

}

void freeScope(Scope* scope)
{
    // TODO
    if(scope != NULL) freeObjectList(scope->objList);
    free(scope);
    scope = NULL;
}

void freeObjectList(ObjectNode *objList)
{
    // TODO
    if(objList!=NULL)
    {
        freeObject(objList->object);
        freeObjectList(objList->next);
    }
    objList = NULL;
}

void freeReferenceList(ObjectNode *objList)
{
    // TODO
    if (objList != NULL)
    {
        freeObject(objList->object);
        freeReferenceList(objList->next);
    }
    objList = NULL;

}

void addObject(ObjectNode **objList, Object* obj)
{
    ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
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

Object* findObject(ObjectNode *objList, char *name)
{
    // TODO
    if(strcmp(objList->object->name,name))
    {
        return objList->object;
    }
    if(objList->next==NULL) return NULL;
    return findObject(objList->next,name);

}

/******************* others ******************************/

void initSymTab(void)
{
    Object* obj;
    Object* param;

    symtab = (SymTab*) malloc(sizeof(SymTab));
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
    addObject(&(obj->procAttrs->paramList),param);
    addObject(&(symtab->globalObjectList), obj);

    obj = createProcedureObject("WRITEC");
    param = createParameterObject("ch", PARAM_VALUE, obj);
    param->paramAttrs->type = makeCharType();
    addObject(&(obj->procAttrs->paramList),param);
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

void enterBlock(Scope* scope)
{
    symtab->currentScope = scope;
}

void exitBlock(void)
{
    symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj)
{
    if (obj->kind == OBJ_PARAMETER)
    {
        Object* owner = symtab->currentScope->owner;
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


