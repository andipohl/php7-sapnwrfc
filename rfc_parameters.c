/**
 * Copyright (c) 2016 - 2020 Gregor Kralik <g.kralik (at) gmail.com>
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * Author: Gregor Kralik <g.kralik (at) gmail.com>
 */

#include "php.h"

#include "string_helper.h"
#include "rfc_parameters.h"
#include "exceptions.h"

#include "sapnwrfc.h"

rfc_set_value_return_t rfc_set_char_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    SAP_UC *value_u;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_STRING) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set CHAR parameter %s, expected string", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    value_u = zval_to_sapuc(value);
    rc = RfcSetChars(h, name, value_u, strlenU(value_u), &error_info);
    free((char *)value_u);

    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set CHAR parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_date_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    SAP_UC *value_u;
    RFC_DATE value_date;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_STRING) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set DATE parameter %s, expected string", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    if (Z_STRLEN_P(value) != SAP_DATE_LN) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set DATE parameter %s, invalid format given", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    value_u = zval_to_sapuc(value);
    memcpy((char *)value_date, (char *)value_u, SAP_DATE_LN * 2);
    free((char *)value_u);

    rc = RfcSetDate(h, name, value_date, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set DATE parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_time_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    SAP_UC *value_u;
    RFC_TIME value_time;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_STRING) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set TIME parameter %s, expected string", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    if (Z_STRLEN_P(value) != SAP_TIME_LN) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set TIME parameter %s, invalid format given", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    value_u = zval_to_sapuc(value);
    memcpy((char *)value_time, (char *)value_u, SAP_TIME_LN * 2);
    free((char *)value_u);

    rc = RfcSetTime(h, name, value_time, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set TIME parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_byte_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value, unsigned max)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_STRING) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set BYTE parameter %s, expected string", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    if (Z_STRLEN_P(value) > max) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set BYTE parameter %s, too long", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    rc = RfcSetBytes(h, name, (SAP_RAW *)Z_STRVAL_P(value), Z_STRLEN_P(value), &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set BYTE parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_table_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_ERROR_INFO error_info;
    RFC_STRUCTURE_HANDLE table_row;
    zend_string *zname;
    HashTable *value_hash;
    zend_string *key;
    zval *val;

    zname = sapuc_to_zend_string(name);

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_ARRAY) {
        zend_error(E_WARNING, "Failed to set TABLE parameter %s, expected array", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    value_hash = Z_ARRVAL_P(value);
    ZEND_HASH_FOREACH_STR_KEY_VAL(value_hash, key, val) {
        table_row = RfcAppendNewRow(h, &error_info);
        if (table_row == NULL) {
            sapnwrfc_throw_function_exception(error_info, "Failed to append TABLE row for parameter %s", ZSTR_VAL(zname));
            zend_string_release(zname);
            return RFC_SET_VALUE_ERROR;
        }

        if (rfc_set_table_row(table_row, val, zname) == RFC_SET_VALUE_ERROR) {
            // exception has been thrown; abort
            zend_string_release(zname);
            return RFC_SET_VALUE_ERROR;
        }
    } ZEND_HASH_FOREACH_END();

    zend_string_release(zname);
    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_num_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value, unsigned max)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    SAP_UC *value_u;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_STRING) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set NUM parameter %s, expected string", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    if (Z_STRLEN_P(value) > max) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set NUM parameter %s, too long",
        ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    value_u = zval_to_sapuc(value);
    rc = RfcSetNum(h, name, (RFC_NUM *)value_u, strlenU(value_u), &error_info);
    free((char *)value_u);

    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set NUM parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_float_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    // if argument type is int, try to convert to double
    if (Z_TYPE_P(value) == IS_LONG) {
        convert_to_double(value);
    }

    if (Z_TYPE_P(value) != IS_DOUBLE) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set FLOAT parameter %s, expected int or double", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    rc = RfcSetFloat(h, name, (RFC_FLOAT)Z_DVAL_P(value), &error_info);

    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set FLOAT parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_int_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_LONG) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set INT parameter %s, expected integer", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    rc = RfcSetInt(h, name, (RFC_INT)Z_LVAL_P(value), &error_info);

    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set INT parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_int1_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_LONG) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set INT1 parameter %s, expected integer", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    if (Z_LVAL_P(value) > 255 || Z_LVAL_P(value) < 0) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set INT1 parameter %s, out of range (0 - 255)", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    rc = RfcSetInt1(h, name, (RFC_INT1)Z_LVAL_P(value), &error_info);

    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set INT1 parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_int2_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_LONG) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set INT2 parameter %s, expected integer", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    if (Z_LVAL_P(value) > 32767 || Z_LVAL_P(value) < -32767) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set INT2 parameter %s, out of range (-32767 - 32767)", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    rc = RfcSetInt2(h, name, (RFC_INT2)Z_LVAL_P(value), &error_info);

    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set INT2 parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_structure_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_STRUCTURE_HANDLE line_handle;
    zend_string *zname;
    rfc_set_value_return_t ret;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    zname = sapuc_to_zend_string(name);

    rc = RfcGetStructure(h, name, &line_handle, &error_info);
    if (rc != RFC_OK) {
        sapnwrfc_throw_function_exception(error_info, "Failed to set STRUCTURE parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    ret = rfc_set_table_row(line_handle, value, zname);
    zend_string_release(zname);
    return ret;
}

rfc_set_value_return_t rfc_set_string_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    SAP_UC *value_u;
    zend_string *zname;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_STRING) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set STRING parameter %s, expected string", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    value_u = zval_to_sapuc(value);
    rc = RfcSetString(h, name, value_u, strlenU(value_u), &error_info);
    free((char *)value_u);

    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set STRING parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_xstring_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_STRING) {
        zname = sapuc_to_zend_string(name);
        zend_error(E_WARNING, "Failed to set XSTRING parameter %s, expected string", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    rc = RfcSetXString(h, name, (SAP_RAW *)Z_STRVAL_P(value), Z_STRLEN_P(value), &error_info);

    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to set XSTRING parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);
        return RFC_SET_VALUE_ERROR;
    }

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_table_row(RFC_STRUCTURE_HANDLE row, zval *value, zend_string *param_name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_FIELD_DESC field_desc;
    RFC_TYPE_DESC_HANDLE type_desc_handle;
    SAP_UC * field_name_u;
    HashTable *value_hash;
    zend_string *key;
    zend_ulong num_key;
    zval *val;

    // if the value is a reference, get the reference value first
    if (Z_ISREF_P(value)) {
        value = Z_REFVAL_P(value);
    }

    if (Z_TYPE_P(value) != IS_ARRAY) {
        zend_error(E_WARNING, "Failed to set TABLE row, expected array for parameter %s", ZSTR_VAL(param_name));
        return RFC_SET_VALUE_ERROR;
    }

    type_desc_handle = RfcDescribeType(row, &error_info);
    if (type_desc_handle == NULL) {
        sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE row type description for parameter %s", ZSTR_VAL(param_name));
        return RFC_SET_VALUE_ERROR;
    }

    value_hash = Z_ARRVAL_P(value);

    ZEND_HASH_FOREACH_KEY_VAL(value_hash, num_key, key, val) {
        if (!key) { // not string
            zend_error(E_WARNING, "All parameter keys must be strings.");
            return RFC_SET_VALUE_ERROR;
        }

        rc = RfcGetFieldDescByName(type_desc_handle, (field_name_u = zend_string_to_sapuc(key)), &field_desc, &error_info);
        if (rc != RFC_OK) {
            free((char *)field_name_u);
            sapnwrfc_throw_function_exception(error_info, "Failed to get description for field %s", ZSTR_VAL(key));
            return RFC_SET_VALUE_ERROR;
        }

        memcpy(field_desc.name, field_name_u, sizeof(RFC_ABAP_NAME)); // RFC_CHAR [30+1]
        free((char *)field_name_u);

        if (rfc_set_field_value(row, field_desc, val) == RFC_SET_VALUE_ERROR) {
            return RFC_SET_VALUE_ERROR;
        }
    } ZEND_HASH_FOREACH_END();

    return RFC_SET_VALUE_OK;
}

rfc_set_value_return_t rfc_set_field_value(DATA_CONTAINER_HANDLE h, RFC_FIELD_DESC field_desc, zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_TABLE_HANDLE table_handle;
    zend_string *zname;
    int ret = RFC_SET_VALUE_OK;

    switch(field_desc.type) {
        case RFCTYPE_CHAR:
            ret = rfc_set_char_value(h, field_desc.name, value);
            break;
        case RFCTYPE_DATE:
            ret = rfc_set_date_value(h, field_desc.name, value);
            break;
        case RFCTYPE_TIME:
            ret = rfc_set_time_value(h, field_desc.name, value);
            break;
        case RFCTYPE_BYTE:
            ret = rfc_set_byte_value(h, field_desc.name, value, field_desc.nucLength);
            break;
        case RFCTYPE_TABLE:
            rc = RfcGetTable(h, field_desc.name, &table_handle, &error_info);
            if (rc != RFC_OK) {
                zname = sapuc_to_zend_string(field_desc.name);
                sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE handle for parameter %s", ZSTR_VAL(zname));
                zend_string_release(zname);
                return RFC_SET_VALUE_ERROR;
            }

            ret = rfc_set_table_value(table_handle, field_desc.name, value);
            break;
        case RFCTYPE_NUM:
            ret = rfc_set_num_value(h, field_desc.name, value, field_desc.nucLength);
            break;
        case RFCTYPE_BCD: // fall through; map to float
        case RFCTYPE_FLOAT:
            ret = rfc_set_float_value(h, field_desc.name, value);
            break;
        case RFCTYPE_INT:
            ret = rfc_set_int_value(h, field_desc.name, value);
            break;
        case RFCTYPE_INT1:
            ret = rfc_set_int1_value(h, field_desc.name, value);
            break;
        case RFCTYPE_INT2:
            ret = rfc_set_int2_value(h, field_desc.name, value);
            break;
        case RFCTYPE_STRUCTURE:
            ret = rfc_set_structure_value(h, field_desc.name, value);
            break;
        case RFCTYPE_STRING:
            ret = rfc_set_string_value(h, field_desc.name, value);
            break;
        case RFCTYPE_XSTRING:
            ret = rfc_set_xstring_value(h, field_desc.name, value);
            break;
        default:
            zname = sapuc_to_zend_string(field_desc.name);
            zend_error(E_WARNING, "Unknown type for parameter %s", ZSTR_VAL(zname));
            zend_string_release(zname);
            ret = RFC_SET_VALUE_ERROR;
    }

    return ret;
}

rfc_set_value_return_t rfc_set_parameter_value(RFC_FUNCTION_HANDLE function_handle,
                               RFC_FUNCTION_DESC_HANDLE function_desc_handle,
                               zend_string *name,
                               zval *value)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_PARAMETER_DESC parameter_desc;
    RFC_TABLE_HANDLE table_handle;
    SAP_UC *parameter_name_u;
    int ret = RFC_SET_VALUE_OK;

    rc = RfcGetParameterDescByName(function_desc_handle,
                                   (parameter_name_u = zend_string_to_sapuc(name)),
                                   &parameter_desc,
                                   &error_info);

    if (rc != RFC_OK) {
        free((char *)parameter_name_u);

        sapnwrfc_throw_function_exception(error_info, "Failed to get description for parameter %s", ZSTR_VAL(name));
        return RFC_SET_VALUE_ERROR;
    }

    switch(parameter_desc.type) {
        case RFCTYPE_CHAR:
            ret = rfc_set_char_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_DATE:
            ret = rfc_set_date_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_TIME:
            ret = rfc_set_time_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_BYTE:
            ret = rfc_set_byte_value(function_handle, parameter_name_u, value, parameter_desc.nucLength);
            break;
        case RFCTYPE_TABLE:
            rc = RfcGetTable(function_handle, parameter_name_u, &table_handle, &error_info);
            if (rc != RFC_OK) {
                free((char *)parameter_name_u);

                sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE handle for parameter %s", ZSTR_VAL(name));
                return RFC_SET_VALUE_ERROR;
            }

            ret = rfc_set_table_value(table_handle, parameter_name_u, value);
            break;
        case RFCTYPE_NUM:
            ret = rfc_set_num_value(function_handle, parameter_name_u, value, parameter_desc.nucLength);
            break;
        case RFCTYPE_BCD: // fall through; map to float
        case RFCTYPE_FLOAT:
            ret = rfc_set_float_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_INT:
            ret = rfc_set_int_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_INT1:
            ret = rfc_set_int1_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_INT2:
            ret = rfc_set_int2_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_STRUCTURE:
            ret = rfc_set_structure_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_STRING:
            ret = rfc_set_string_value(function_handle, parameter_name_u, value);
            break;
        case RFCTYPE_XSTRING:
            ret = rfc_set_xstring_value(function_handle, parameter_name_u, value);
            break;
        default:
            zend_error(E_WARNING, "Unknown type for parameter %s", ZSTR_VAL(name));
            ret = RFC_SET_VALUE_ERROR;
    }

    free((char *)parameter_name_u);
    return ret;
}

zval rfc_get_char_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, unsigned len, unsigned char rtrim_enabled)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    SAP_UC *buf;
    zend_string *zname;
    zval value;

    buf = (SAP_UC *)mallocU(len);

    rc = RfcGetChars(h, name, buf, len, &error_info);
    if (rc != RFC_OK) {
        free(buf);
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get CHAR parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    value = sapuc_to_zval_len_ex(buf, len, rtrim_enabled);
    free(buf);

    return value;
}

zval rfc_get_date_value(DATA_CONTAINER_HANDLE h, SAP_UC *name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_DATE buf;
    zend_string *zname;
    zval value;

    rc = RfcGetDate(h, name, buf, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get DATE parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    value = sapuc_to_zval_len(buf, SAP_DATE_LN);

    return value;
}

zval rfc_get_time_value(DATA_CONTAINER_HANDLE h, SAP_UC *name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_TIME buf;
    zend_string *zname;
    zval value;

    rc = RfcGetTime(h, name, buf, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get TIME parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    value = sapuc_to_zval_len(buf, SAP_TIME_LN);

    return value;
}

zval rfc_get_byte_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, unsigned len)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    SAP_RAW *buf;
    zval value;

    buf = (SAP_RAW *)malloc(len);
    rc = RfcGetBytes(h, name, buf, len, &error_info);
    if (rc != RFC_OK) {
        free(buf);

        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get BYTE parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    ZVAL_STRINGL(&value, (char *)buf, len);
    free(buf);

    return value;
}

zval rfc_get_table_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, unsigned char rtrim_enabled)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    RFC_STRUCTURE_HANDLE line_handle;
    zval value;
    zval line_value;
    unsigned table_len = 0;
    unsigned i = 0;

    zname = sapuc_to_zend_string(name);

    rc = RfcGetRowCount(h, &table_len, &error_info);
    if (rc != RFC_OK) {
        sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    array_init(&value);
    for(i = 0; i < table_len; i++) {
        RfcMoveTo(h, i, NULL);
        line_handle = RfcGetCurrentRow(h, NULL);

        line_value = rfc_get_table_line(line_handle, zname, rtrim_enabled);
        if (ZVAL_IS_NULL(&line_value)) {
            // error; exception has been thrown
            ZVAL_NULL(&value);
            zend_string_release(zname);
            return value;
        }
        add_next_index_zval(&value, &line_value);
    }

    zend_string_release(zname);
    return value;
}

zval rfc_get_num_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, unsigned len)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    RFC_NUM *buf;
    zval value;

    buf = (RFC_NUM *)mallocU(len);

    rc = RfcGetNum(h, name, buf, len, &error_info);
    if (rc != RFC_OK) {
        free(buf);

        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get NUM parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    value = sapuc_to_zval_len(buf, len);
    free(buf);

    return value;
}

zval rfc_get_float_value(DATA_CONTAINER_HANDLE h, SAP_UC *name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    RFC_FLOAT buf;
    zval value;

    rc = RfcGetFloat(h, name, &buf, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get FLOAT parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    ZVAL_DOUBLE(&value, (double)buf);

    return value;
}

zval rfc_get_int_value(DATA_CONTAINER_HANDLE h, SAP_UC *name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    RFC_INT buf;
    zval value;

    rc = RfcGetInt(h, name, &buf, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get INT parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    ZVAL_LONG(&value, buf);

    return value;
}

zval rfc_get_int1_value(DATA_CONTAINER_HANDLE h, SAP_UC *name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    RFC_INT1 buf;
    zval value;

    rc = RfcGetInt1(h, name, &buf, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get INT1 parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    ZVAL_LONG(&value, (int)buf);

    return value;
}

zval rfc_get_int2_value(DATA_CONTAINER_HANDLE h, SAP_UC *name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    RFC_INT2 buf;
    zval value;

    rc = RfcGetInt2(h, name, &buf, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get INT2 parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    ZVAL_LONG(&value, (int)buf);

    return value;
}

zval rfc_get_structure_value(DATA_CONTAINER_HANDLE h, SAP_UC *name, unsigned char rtrim_enabled)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    RFC_STRUCTURE_HANDLE line_handle;
    zval value;

    zname = sapuc_to_zend_string(name);

    rc = RfcGetStructure(h, name, &line_handle, &error_info);
    if (rc != RFC_OK) {
        sapnwrfc_throw_function_exception(error_info, "Failed to get STRUCTURE parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    value = rfc_get_table_line(line_handle, zname, rtrim_enabled);
    zend_string_release(zname);

    return value;
}

zval rfc_get_string_value(DATA_CONTAINER_HANDLE h, SAP_UC *name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    zval value;
    unsigned str_len, result_len;
    SAP_UC *buf;

    rc = RfcGetStringLength(h, name, &str_len, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get length of STRING parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    if (str_len == 0) {
        ZVAL_STRING(&value, "");
        return value;
    }

    buf = (SAP_UC *)mallocU(str_len + 1);
    rc = RfcGetString(h, name, buf, str_len + 1, &result_len, &error_info);
    if (rc != RFC_OK) {
        free(buf);

        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get STRING parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    value = sapuc_to_zval_len(buf, result_len);
    free(buf);

    return value;
}

zval rfc_get_xstring_value(DATA_CONTAINER_HANDLE h, SAP_UC *name)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    zval value;
    unsigned str_len, result_len;
    SAP_RAW *buf;

    rc = RfcGetStringLength(h, name, &str_len, &error_info);
    if (rc != RFC_OK) {
        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get length of XSTRING parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    if (str_len == 0) {
        ZVAL_STRING(&value, "");
        return value;
    }

    buf = (SAP_RAW *)malloc(str_len + 1);
    buf[str_len] = '\0';

    rc = RfcGetXString(h, name, buf, str_len, &result_len, &error_info);
    if (rc != RFC_OK) {
        free(buf);

        zname = sapuc_to_zend_string(name);
        sapnwrfc_throw_function_exception(error_info, "Failed to get XSTRING parameter %s", ZSTR_VAL(zname));
        zend_string_release(zname);

        ZVAL_NULL(&value);
        return value;
    }

    ZVAL_STRINGL(&value, (char *)buf, result_len);
    free(buf);

    return value;
}

zval rfc_get_table_line(RFC_STRUCTURE_HANDLE line, zend_string *param_name, unsigned char rtrim_enabled)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_TYPE_DESC_HANDLE type_handle;
    RFC_FIELD_DESC field_desc;
    unsigned field_count = 0;
    unsigned i = 0;
    zval value;
    zval field_value;
    zend_string *tmp;

    type_handle = RfcDescribeType(line, &error_info);
    if (type_handle == NULL) {
        sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE line for parameter %s", ZSTR_VAL(param_name));

        ZVAL_NULL(&value);
        return value;
    }

    rc = RfcGetFieldCount(type_handle, &field_count, &error_info);
    if (rc != RFC_OK) {
        sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE line field count for parameter %s", ZSTR_VAL(param_name));

        ZVAL_NULL(&value);
        return value;
    }

    array_init(&value);

    for(i = 0; i < field_count; i++) {
        rc = RfcGetFieldDescByIndex(type_handle, i, &field_desc, &error_info);
        if (rc != RFC_OK) {
            sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE line field description for parameter %s", ZSTR_VAL(param_name));

            ZVAL_NULL(&value);
            return value;
        }

        field_value = rfc_get_field_value(line, field_desc, rtrim_enabled);
        if (ZVAL_IS_NULL(&field_value)) {
            // error; exception has been thrown
            ZVAL_NULL(&value);
            return value;
        }

        tmp = sapuc_to_zend_string(field_desc.name);
        add_assoc_zval(&value, ZSTR_VAL(tmp), &field_value);
        zend_string_release(tmp);
    }

    return value;
}

zval rfc_get_field_value(RFC_STRUCTURE_HANDLE h, RFC_FIELD_DESC field_desc, unsigned char rtrim_enabled)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    zend_string *zname;
    RFC_TABLE_HANDLE table_handle;
    zval value;

    switch(field_desc.type) {
        case RFCTYPE_DECF34:
        case RFCTYPE_CHAR:
            value = rfc_get_char_value(h, field_desc.name, field_desc.nucLength, rtrim_enabled);
            break;
        case RFCTYPE_DATE:
            value = rfc_get_date_value(h, field_desc.name);
            break;
        case RFCTYPE_TIME:
            value = rfc_get_time_value(h, field_desc.name);
            break;
        case RFCTYPE_BYTE:
            value = rfc_get_byte_value(h, field_desc.name, field_desc.nucLength);
            break;
        case RFCTYPE_TABLE:
            rc = RfcGetTable(h, field_desc.name, &table_handle, &error_info);
            if (rc != RFC_OK) {
                zname = sapuc_to_zend_string(field_desc.name);
                sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE handle for parameter %s", ZSTR_VAL(zname));
                zend_string_release(zname);

                ZVAL_NULL(&value);
                return value;
            }

            value = rfc_get_table_value(table_handle, field_desc.name, rtrim_enabled);
            break;
        case RFCTYPE_NUM:
            value = rfc_get_num_value(h, field_desc.name, field_desc.nucLength);
            break;
        case RFCTYPE_BCD: // fall through; map to float
        case RFCTYPE_FLOAT:
            value = rfc_get_float_value(h, field_desc.name);
            break;
        case RFCTYPE_INT:
            value = rfc_get_int_value(h, field_desc.name);
            break;
        case RFCTYPE_INT1:
            value = rfc_get_int1_value(h, field_desc.name);
            break;
        case RFCTYPE_INT2:
            value = rfc_get_int2_value(h, field_desc.name);
            break;
        case RFCTYPE_STRUCTURE:
            value = rfc_get_structure_value(h, field_desc.name, rtrim_enabled);
            break;
        case RFCTYPE_STRING:
            value = rfc_get_string_value(h, field_desc.name);
            break;
        case RFCTYPE_XSTRING:
            value = rfc_get_xstring_value(h, field_desc.name);
            break;
        default:
            zname = sapuc_to_zend_string(field_desc.name);
            zend_error(E_ERROR, "Unknown type for parameter %s", ZSTR_VAL(zname));
            zend_string_release(zname);
            ZVAL_NULL(&value);
    }

    return value;
}

zval rfc_get_parameter_value(RFC_FUNCTION_HANDLE function_handle,
                                               RFC_FUNCTION_DESC_HANDLE function_desc_handle,
                                               zend_string *name,
                                               unsigned char rtrim_enabled)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_PARAMETER_DESC parameter_desc;
    RFC_TABLE_HANDLE table_handle;
    SAP_UC *parameter_name_u;
    zval value;

    rc = RfcGetParameterDescByName(function_desc_handle, (parameter_name_u = zend_string_to_sapuc(name)), &parameter_desc, &error_info);
    if (rc != RFC_OK) {
        free((char *)parameter_name_u);

        sapnwrfc_throw_function_exception(error_info, "Failed to get description for parameter %s", ZSTR_VAL(name));

        ZVAL_NULL(&value);
        return value;
    }

    switch(parameter_desc.type) {
        case RFCTYPE_CHAR:
            value = rfc_get_char_value(function_handle, parameter_name_u, parameter_desc.nucLength, rtrim_enabled);
            break;
        case RFCTYPE_DATE:
            value = rfc_get_date_value(function_handle, parameter_name_u);
            break;
        case RFCTYPE_TIME:
            value = rfc_get_time_value(function_handle, parameter_name_u);
            break;
        case RFCTYPE_BYTE:
            value = rfc_get_byte_value(function_handle, parameter_name_u, parameter_desc.nucLength);
            break;
        case RFCTYPE_TABLE:
            rc = RfcGetTable(function_handle, parameter_name_u, &table_handle, &error_info);
            if (rc != RFC_OK) {
                free((char *)parameter_name_u);

                sapnwrfc_throw_function_exception(error_info, "Failed to get TABLE handle for parameter %s", ZSTR_VAL(name));

                ZVAL_NULL(&value);
            }
            value = rfc_get_table_value(table_handle, parameter_name_u, rtrim_enabled);
            break;
        case RFCTYPE_NUM:
            value = rfc_get_num_value(function_handle, parameter_name_u, parameter_desc.nucLength);
            break;
        case RFCTYPE_BCD: // fall through; map to float
        case RFCTYPE_FLOAT:
            value = rfc_get_float_value(function_handle, parameter_name_u);
            break;
        case RFCTYPE_INT:
            value = rfc_get_int_value(function_handle, parameter_name_u);
            break;
        case RFCTYPE_INT1:
            value = rfc_get_int1_value(function_handle, parameter_name_u);
            break;
        case RFCTYPE_INT2:
            value = rfc_get_int2_value(function_handle, parameter_name_u);
            break;
        case RFCTYPE_STRUCTURE:
            value = rfc_get_structure_value(function_handle, parameter_name_u, rtrim_enabled);
            break;
        case RFCTYPE_STRING:
            value = rfc_get_string_value(function_handle, parameter_name_u);
            break;
        case RFCTYPE_XSTRING:
            value = rfc_get_xstring_value(function_handle, parameter_name_u);
            break;
        default:
            zend_error(E_ERROR, "Unknown type for parameter %s", ZSTR_VAL(name));
            ZVAL_NULL(&value);
    }

    free((char *)parameter_name_u);

    return value;
}

static int rfc_describe_type(RFC_TYPE_DESC_HANDLE type_desc_handle, zval *type_description);
static int rfc_wrap_parameter_description(RFC_PARAMETER_DESC parameter_desc, zval *parameter_description);
static int rfc_wrap_field_description(RFC_FIELD_DESC field_desc, zval *type_description);

static int rfc_describe_type(RFC_TYPE_DESC_HANDLE type_desc_handle, zval *type_description)
{
    unsigned int field_count;
    zend_string *field_name;
    zval field_description;

    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_FIELD_DESC field_desc;

    rc = RfcGetFieldCount(type_desc_handle, &field_count, &error_info);
    if (rc != RFC_OK) {
        return -1;
    }

    array_init(type_description);

    // loop over the fields and get their description
    for (unsigned int field_idx = 0; field_idx < field_count; field_idx++) {
        rc = RfcGetFieldDescByIndex(type_desc_handle, field_idx, &field_desc, &error_info);
        if (rc != RFC_OK) {
            return -1;
        }

        // wrap the field description in an array
        if (rfc_wrap_field_description(field_desc, &field_description) == -1) {
            return -1;
        }

        // add this field's description to the type description
        field_name = sapuc_to_zend_string(field_desc.name);
        add_assoc_zval(type_description, ZSTR_VAL(field_name), &field_description);
        zend_string_release(field_name);
    }

    return 0;
}

static int rfc_wrap_parameter_description(RFC_PARAMETER_DESC parameter_desc, zval *parameter_description)
{
    zval type_description;

    // create array with parameter description
    array_init(parameter_description);
    add_assoc_str(parameter_description, "name", sapuc_to_zend_string(parameter_desc.name));
    add_assoc_str(parameter_description, "type", sapuc_to_zend_string((SAP_UC *)RfcGetTypeAsString(parameter_desc.type)));
    add_assoc_str(parameter_description, "direction", sapuc_to_zend_string((SAP_UC *)RfcGetDirectionAsString(parameter_desc.direction)));
    add_assoc_str(parameter_description, "description", sapuc_to_zend_string(parameter_desc.parameterText));
    add_assoc_long(parameter_description, "ucLength", parameter_desc.ucLength);
    add_assoc_long(parameter_description, "nucLength", parameter_desc.nucLength);
    add_assoc_long(parameter_description, "decimals", parameter_desc.decimals);
    add_assoc_bool(parameter_description, "optional", parameter_desc.optional);
    add_assoc_str(parameter_description, "default", sapuc_to_zend_string(parameter_desc.defaultValue));
    // TODO extendedDescription?

    // for structures and tables, there is additional type information available
    if (parameter_desc.typeDescHandle) {
        if (rfc_describe_type(parameter_desc.typeDescHandle, &type_description) == -1) {
            return -1;
        }

        add_assoc_zval(parameter_description, "typedef", &type_description);
    }

    return 0;
}

static int rfc_wrap_field_description(RFC_FIELD_DESC field_desc, zval *type_description)
{
    zval type_def;

    array_init(type_description);
    add_assoc_str(type_description, "name", sapuc_to_zend_string(field_desc.name));
    add_assoc_str(type_description, "type", sapuc_to_zend_string((SAP_UC *)RfcGetTypeAsString(field_desc.type)));
    add_assoc_long(type_description, "ucLength", field_desc.ucLength);
    add_assoc_long(type_description, "ucOffset", field_desc.ucOffset);
    add_assoc_long(type_description, "nucLength", field_desc.nucLength);
    add_assoc_long(type_description, "nucOffset", field_desc.nucOffset);
    add_assoc_long(type_description, "decimals", field_desc.decimals);
    // TODO extendedDescription?

    // if this field is a complex type, describe it
    if (field_desc.typeDescHandle) {
        if (rfc_describe_type(field_desc.typeDescHandle, &type_def) == -1) {
            return -1;
        }

        add_assoc_zval(type_description, "typedef", &type_def);
    }

    return 0;
}

int rfc_describe_function_interface(RFC_FUNCTION_DESC_HANDLE function_desc_handle, zval *function_description)
{
    RFC_RC rc = RFC_OK;
    RFC_ERROR_INFO error_info;
    RFC_PARAMETER_DESC parameter_desc;

    unsigned int param_count = 0;
    zval parameter_description;
    zend_string *parameter_name;

    // get the number of function parameters
    rc = RfcGetParameterCount(function_desc_handle, &param_count, &error_info);
    if (rc != RFC_OK) {
        return -1;
    }

    array_init(function_description);

    // loop over the parameters and get type information
    for (int param_idx = 0; param_idx < param_count; param_idx++) {
        rc = RfcGetParameterDescByIndex(function_desc_handle, param_idx, &parameter_desc, &error_info);
        if (rc != RFC_OK) {
            return -1;
        }

        // wrap the type information in an array
        if (rfc_wrap_parameter_description(parameter_desc, &parameter_description) == -1) {
            return -1;
        }

        // add array key with param name and desription
        parameter_name = sapuc_to_zend_string(parameter_desc.name);
        add_assoc_zval(function_description, ZSTR_VAL(parameter_name), &parameter_description);
        zend_string_release(parameter_name);
    }

    return 0;
}
