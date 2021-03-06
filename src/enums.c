/* The MIT License (MIT)
 *
 * Copyright (c) 2017 Stefano Trettel
 *
 * Software repository: MoonVulkan, https://github.com/stetre/moonvulkan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "internal.h"

/*------------------------------------------------------------------------------*
 | Code<->string map for enumerations                                           |
 *------------------------------------------------------------------------------*/


/* code <-> string record */
#define rec_t struct rec_s
struct rec_s {
    RB_ENTRY(rec_s) CodeEntry;
    RB_ENTRY(rec_s) StringEntry;
    uint32_t domain;
    uint32_t code;  /* (domain, code) = search key in code tree */
    char     *str;  /* (domain, str) = search key in string tree */
};

/* compare functions */
static int cmp_code(rec_t *rec1, rec_t *rec2) 
    { 
    if(rec1->domain != rec2->domain)
        return (rec1->domain < rec2->domain ? -1 : rec1->domain > rec2->domain);
    return (rec1->code < rec2->code ? -1 : rec1->code > rec2->code);
    } 

static int cmp_str(rec_t *rec1, rec_t *rec2) 
    { 
    if(rec1->domain != rec2->domain)
        return (rec1->domain < rec2->domain ? -1 : rec1->domain > rec2->domain);
    return strcmp(rec1->str, rec2->str);
    } 

static RB_HEAD(CodeTree, rec_s) CodeHead = RB_INITIALIZER(&CodeHead);
RB_PROTOTYPE_STATIC(CodeTree, rec_s, CodeEntry, cmp_code) 
RB_GENERATE_STATIC(CodeTree, rec_s, CodeEntry, cmp_code) 

static RB_HEAD(StringTree, rec_s) StringHead = RB_INITIALIZER(&StringHead);
RB_PROTOTYPE_STATIC(StringTree, rec_s, StringEntry, cmp_str) 
RB_GENERATE_STATIC(StringTree, rec_s, StringEntry, cmp_str) 
 
static rec_t *code_remove(rec_t *rec) 
    { return RB_REMOVE(CodeTree, &CodeHead, rec); }
static rec_t *code_insert(rec_t *rec) 
    { return RB_INSERT(CodeTree, &CodeHead, rec); }
static rec_t *code_search(uint32_t domain, uint32_t code) 
    { rec_t tmp; tmp.domain = domain; tmp.code = code; return RB_FIND(CodeTree, &CodeHead, &tmp); }
static rec_t *code_first(uint32_t domain, uint32_t code) 
    { rec_t tmp; tmp.domain = domain; tmp.code = code; return RB_NFIND(CodeTree, &CodeHead, &tmp); }
static rec_t *code_next(rec_t *rec)
    { return RB_NEXT(CodeTree, &CodeHead, rec); }
#if 0
static rec_t *code_prev(rec_t *rec)
    { return RB_PREV(CodeTree, &CodeHead, rec); }
static rec_t *code_min(void)
    { return RB_MIN(CodeTree, &CodeHead); }
static rec_t *code_max(void)
    { return RB_MAX(CodeTree, &CodeHead); }
static rec_t *code_root(void)
    { return RB_ROOT(&CodeHead); }
#endif
 
static rec_t *str_remove(rec_t *rec) 
    { return RB_REMOVE(StringTree, &StringHead, rec); }
static rec_t *str_insert(rec_t *rec) 
    { return RB_INSERT(StringTree, &StringHead, rec); }
static rec_t *str_search(uint32_t domain, const char* str) 
    { rec_t tmp; tmp.domain = domain; tmp.str = (char*)str; return RB_FIND(StringTree, &StringHead, &tmp); }
#if 0
static rec_t *str_first(uint32_t domain, const char* str ) 
    { rec_t tmp; tmp.domain = domain; tmp.str = str; return RB_NFIND(StringTree, &StringHead, &tmp); }
static rec_t *str_next(rec_t *rec)
    { return RB_NEXT(StringTree, &StringHead, rec); }
static rec_t *str_prev(rec_t *rec)
    { return RB_PREV(StringTree, &StringHead, rec); }
static rec_t *str_min(void)
    { return RB_MIN(StringTree, &StringHead); }
static rec_t *str_max(void)
    { return RB_MAX(StringTree, &StringHead); }
static rec_t *str_root(void)
    { return RB_ROOT(&StringHead); }
#endif


static int enums_new(lua_State *L, uint32_t domain, uint32_t code, const char *str)
    {
    rec_t *rec;
    if((rec = (rec_t*)Malloc(L, sizeof(rec_t))) == NULL) return errmemory(L);

    memset(rec, 0, sizeof(rec_t));
    rec->domain = domain;
    rec->code = code;
    rec->str = Strdup(L, str);
    if(code_search(domain, code) || str_search(domain, str))
        { 
        Free(L, rec->str);
        Free(L, rec); 
        printf("enum domain:%u value:'%s'(%u)\n", domain, str, code);
        return unexpected(L); /* duplicate value */
        }
    code_insert(rec);
    str_insert(rec);
    return 0;
    }

static void enums_free(lua_State *L, rec_t* rec)
    {
    if(code_search(rec->domain, rec->code) == rec)
        code_remove(rec);
    if(str_search(rec->domain, rec->str) == rec)
        str_remove(rec);
    Free(L, rec->str);
    Free(L, rec);   
    }

void enums_free_all(lua_State *L)
    {
    rec_t *rec;
    while((rec = code_first(0, 0)))
        enums_free(L, rec);
    }

#if 0
uint32_t enums_code(uint32_t domain, const char *str, int* found)
    {
    rec_t *rec = str_search(domain, str);
    if(!rec)
        { *found = 0; return 0; }
    *found = 1; 
    return rec->code;
    }

const char* enums_string(uint32_t domain, uint32_t code)
    {
    rec_t *rec = code_search(domain, code);
    if(!rec)
        return NULL;
    return rec->str;
    }

#endif


uint32_t enums_test(lua_State *L, uint32_t domain, int arg, int *err)
    {
    rec_t *rec;
    const char *s = luaL_optstring(L, arg, NULL);

    if(!s)
        { *err = ERR_NOTPRESENT; return 0; }

    rec = str_search(domain, s);
    if(!rec)
        { *err = ERR_VALUE; return 0; }
    
    *err = ERR_SUCCESS;
    return rec->code;
    }

uint32_t enums_check(lua_State *L, uint32_t domain, int arg)
    {
    rec_t *rec;
    const char *s = luaL_checkstring(L, arg);

    rec = str_search(domain, s);
    if(!rec)
        return luaL_argerror(L, arg, badvalue(L, s));
    
    return rec->code;
    }

int enums_push(lua_State *L, uint32_t domain, uint32_t code)
    {
    rec_t *rec = code_search(domain, code);

    if(!rec)
        return unexpected(L);

    lua_pushstring(L, rec->str);
    return 1;
    }

int enums_values(lua_State *L, uint32_t domain)
    {
    int i;
    rec_t *rec;

    lua_newtable(L);
    i = 1;
    rec = code_first(domain, 0);
    while(rec)
        {
        if(rec->domain == domain)
            {
            lua_pushstring(L, rec->str);
            lua_rawseti(L, -2, i++);
            }
        rec = code_next(rec);
        }

    return 1;
    }


uint32_t* enums_checklist(lua_State *L, uint32_t domain, int arg, uint32_t *count, int *err)
    {
    uint32_t* list;
    uint32_t i;

    *count = 0;
    *err = 0;
    if(lua_isnoneornil(L, arg))
        { *err = ERR_NOTPRESENT; return NULL; }
    if(lua_type(L, arg) != LUA_TTABLE)
        { *err = ERR_TABLE; return NULL; }

    *count = luaL_len(L, arg);
    if(*count == 0)
        { *err = ERR_NOTPRESENT; return NULL; }

    list = (uint32_t*)MallocNoErr(L, sizeof(uint32_t) * (*count));
    if(!list)
        { *count = 0; *err = ERR_MEMORY; return NULL; }

    for(i=0; i<*count; i++)
        {
        lua_rawgeti(L, arg, i+1);
        list[i] = enums_test(L, domain, -1, err);
        lua_pop(L, 1);
        if(*err)
            { Free(L, list); *count = 0; return NULL; }
        }
    return list;
    }

void enums_freelist(lua_State *L, uint32_t *list)
    {
    if(!list)
        return;
    Free(L, list);
    }

/*------------------------------------------------------------------------------*
 |                                                                              |
 *------------------------------------------------------------------------------*/

static int Enum(lua_State *L)
/* { strings } = vk.enum('type') lists all the values for a given enum type */
    { 
    const char *s = luaL_checkstring(L, 1); 
#define CASE(xxx) if(strcmp(s, ""#xxx) == 0) return values##xxx(L)
    CASE(type);
    CASE(result);
    CASE(subpasscontents);
    CASE(indextype);
    CASE(format);
    CASE(commandbufferlevel);
    CASE(pipelinebindpoint);
    CASE(attachmentstoreop);
    CASE(attachmentloadop);
    CASE(descriptortype);
    CASE(bordercolor);
    CASE(sampleraddressmode);
    CASE(samplermipmapmode);
    CASE(filter);
    CASE(dynamicstate);
    CASE(blendop);
    CASE(blendfactor);
    CASE(logicop);
    CASE(stencilop);
    CASE(compareop);
    CASE(frontface);
    CASE(polygonmode);
    CASE(primitivetopology);
    CASE(vertexinputrate);
    CASE(componentswizzle);
    CASE(imageviewtype);
    CASE(imagelayout);
    CASE(sharingmode);
    CASE(querytype);
    CASE(physicaldevicetype);
    CASE(imagetiling);
    CASE(imagetype);
    CASE(presentmode);
    CASE(colorspace);
    CASE(debugreportobjecttype);
    CASE(descriptorupdatetemplatetype);
    CASE(objecttype);
    CASE(blendoverlap);
    CASE(samplerreductionmode);
    CASE(validationcheck);
    CASE(discardrectanglemode);
    CASE(displaypowerstate);
    CASE(deviceeventtype);
    CASE(displayeventtype);
    CASE(pointclippingbehavior);
    CASE(tessellationdomainorigin);
    CASE(samplerycbcrmodelconversion);
    CASE(samplerycbcrrange);
    CASE(chromalocation);
    CASE(validationcacheheaderversion);
    CASE(queueglobalpriority);
    CASE(conservativerasterizationmode);
    CASE(vendorid);
    CASE(driverid);
    CASE(timedomain);
#undef CASE
    return 0;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "enum", Enum },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_enums(lua_State *L)
    {
    uint32_t domain;

    luaL_setfuncs(L, Functions, 0);

    /* Add all the code<->string mappings and the vk.VK_XXX constant strings */
#define ADD(what, s) do { enums_new(L, domain, NONVK_##what, s); } while(0)

    domain = DOMAIN_NONVK_TYPE; 
    ADD(TYPE_INT8, "int8");
    ADD(TYPE_UINT8, "uint8");
    ADD(TYPE_INT16, "int16");
    ADD(TYPE_UINT16, "uint16");
    ADD(TYPE_INT32, "int32");
    ADD(TYPE_UINT32, "uint32");
    ADD(TYPE_INT64, "int64");
    ADD(TYPE_UINT64, "uint64");
    ADD(TYPE_BYTE, "byte");
    ADD(TYPE_UBYTE, "ubyte");
    ADD(TYPE_SHORT, "short");
    ADD(TYPE_USHORT, "ushort");
    ADD(TYPE_INT, "int");
    ADD(TYPE_UINT, "uint");
    ADD(TYPE_LONG, "long");
    ADD(TYPE_ULONG, "ulong");
    ADD(TYPE_FLOAT, "float");
    ADD(TYPE_DOUBLE, "double");
#undef ADD
#define ADD(what, s) do {                               \
    lua_pushstring(L, s); lua_setfield(L, -2, #what);   \
    enums_new(L, domain, VK_##what, s);                 \
} while(0)

    domain = DOMAIN_RESULT; /* VkResult */
    ADD(SUCCESS, "success");
    ADD(NOT_READY, "not ready");
    ADD(TIMEOUT, "timeout");
    ADD(EVENT_SET, "event set");
    ADD(EVENT_RESET, "event reset");
    ADD(INCOMPLETE, "incomplete");
    ADD(ERROR_OUT_OF_HOST_MEMORY, "out of host memory");
    ADD(ERROR_OUT_OF_DEVICE_MEMORY, "out of device memory");
    ADD(ERROR_INITIALIZATION_FAILED, "initialization failed");
    ADD(ERROR_DEVICE_LOST, "device lost");
    ADD(ERROR_MEMORY_MAP_FAILED, "memory map failed");
    ADD(ERROR_LAYER_NOT_PRESENT, "layer not present");
    ADD(ERROR_EXTENSION_NOT_PRESENT, "extension not present");
    ADD(ERROR_FEATURE_NOT_PRESENT, "feature not present");
    ADD(ERROR_INCOMPATIBLE_DRIVER, "incompatible driver");
    ADD(ERROR_TOO_MANY_OBJECTS, "too many objects");
    ADD(ERROR_FORMAT_NOT_SUPPORTED, "format not supported");
    ADD(ERROR_FRAGMENTED_POOL, "fragmented pool");
    ADD(ERROR_SURFACE_LOST_KHR, "surface lost");
    ADD(ERROR_NATIVE_WINDOW_IN_USE_KHR, "native window in use");
    ADD(SUBOPTIMAL_KHR, "suboptimal");
    ADD(ERROR_OUT_OF_DATE_KHR, "out of date");
    ADD(ERROR_INCOMPATIBLE_DISPLAY_KHR, "incompatible display");
    ADD(ERROR_VALIDATION_FAILED_EXT, "validation failed");
    ADD(ERROR_OUT_OF_POOL_MEMORY, "out of pool memory");
    ADD(ERROR_INVALID_EXTERNAL_HANDLE, "invalid external handle");
    ADD(ERROR_NOT_PERMITTED_EXT, "not permitted");
    ADD(ERROR_FRAGMENTATION_EXT, "fragmentation");
    ADD(ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT, "invalid drm format modifier plane layout");

    domain = DOMAIN_PIPELINE_CACHE_HEADER_VERSION; /* VkPipelineCacheHeaderVersion */
    ADD(PIPELINE_CACHE_HEADER_VERSION_ONE, "one");

    domain = DOMAIN_SYSTEM_ALLOCATION_SCOPE; /* VkSystemAllocationScope */
    ADD(SYSTEM_ALLOCATION_SCOPE_COMMAND, "command");
    ADD(SYSTEM_ALLOCATION_SCOPE_OBJECT, "object");
    ADD(SYSTEM_ALLOCATION_SCOPE_CACHE, "cache");
    ADD(SYSTEM_ALLOCATION_SCOPE_DEVICE, "device");
    ADD(SYSTEM_ALLOCATION_SCOPE_INSTANCE, "instance");

    domain = DOMAIN_INTERNAL_ALLOCATION_TYPE; /* VkInternalAllocationType */
    ADD(INTERNAL_ALLOCATION_TYPE_EXECUTABLE, "executable");

    domain = DOMAIN_FORMAT; /* VkFormat */
    ADD(FORMAT_UNDEFINED, "undefined");
    ADD(FORMAT_R4G4_UNORM_PACK8, "r4g4 unorm pack8");
    ADD(FORMAT_R4G4B4A4_UNORM_PACK16, "r4g4b4a4 unorm pack16");
    ADD(FORMAT_B4G4R4A4_UNORM_PACK16, "b4g4r4a4 unorm pack16");
    ADD(FORMAT_R5G6B5_UNORM_PACK16, "r5g6b5 unorm pack16");
    ADD(FORMAT_B5G6R5_UNORM_PACK16, "b5g6r5 unorm pack16");
    ADD(FORMAT_R5G5B5A1_UNORM_PACK16, "r5g5b5a1 unorm pack16");
    ADD(FORMAT_B5G5R5A1_UNORM_PACK16, "b5g5r5a1 unorm pack16");
    ADD(FORMAT_A1R5G5B5_UNORM_PACK16, "a1r5g5b5 unorm pack16");
    ADD(FORMAT_R8_UNORM, "r8 unorm");
    ADD(FORMAT_R8_SNORM, "r8 snorm");
    ADD(FORMAT_R8_USCALED, "r8 uscaled");
    ADD(FORMAT_R8_SSCALED, "r8 sscaled");
    ADD(FORMAT_R8_UINT, "r8 uint");
    ADD(FORMAT_R8_SINT, "r8 sint");
    ADD(FORMAT_R8_SRGB, "r8 srgb");
    ADD(FORMAT_R8G8_UNORM, "r8g8 unorm");
    ADD(FORMAT_R8G8_SNORM, "r8g8 snorm");
    ADD(FORMAT_R8G8_USCALED, "r8g8 uscaled");
    ADD(FORMAT_R8G8_SSCALED, "r8g8 sscaled");
    ADD(FORMAT_R8G8_UINT, "r8g8 uint");
    ADD(FORMAT_R8G8_SINT, "r8g8 sint");
    ADD(FORMAT_R8G8_SRGB, "r8g8 srgb");
    ADD(FORMAT_R8G8B8_UNORM, "r8g8b8 unorm");
    ADD(FORMAT_R8G8B8_SNORM, "r8g8b8 snorm");
    ADD(FORMAT_R8G8B8_USCALED, "r8g8b8 uscaled");
    ADD(FORMAT_R8G8B8_SSCALED, "r8g8b8 sscaled");
    ADD(FORMAT_R8G8B8_UINT, "r8g8b8 uint");
    ADD(FORMAT_R8G8B8_SINT, "r8g8b8 sint");
    ADD(FORMAT_R8G8B8_SRGB, "r8g8b8 srgb");
    ADD(FORMAT_B8G8R8_UNORM, "b8g8r8 unorm");
    ADD(FORMAT_B8G8R8_SNORM, "b8g8r8 snorm");
    ADD(FORMAT_B8G8R8_USCALED, "b8g8r8 uscaled");
    ADD(FORMAT_B8G8R8_SSCALED, "b8g8r8 sscaled");
    ADD(FORMAT_B8G8R8_UINT, "b8g8r8 uint");
    ADD(FORMAT_B8G8R8_SINT, "b8g8r8 sint");
    ADD(FORMAT_B8G8R8_SRGB, "b8g8r8 srgb");
    ADD(FORMAT_R8G8B8A8_UNORM, "r8g8b8a8 unorm");
    ADD(FORMAT_R8G8B8A8_SNORM, "r8g8b8a8 snorm");
    ADD(FORMAT_R8G8B8A8_USCALED, "r8g8b8a8 uscaled");
    ADD(FORMAT_R8G8B8A8_SSCALED, "r8g8b8a8 sscaled");
    ADD(FORMAT_R8G8B8A8_UINT, "r8g8b8a8 uint");
    ADD(FORMAT_R8G8B8A8_SINT, "r8g8b8a8 sint");
    ADD(FORMAT_R8G8B8A8_SRGB, "r8g8b8a8 srgb");
    ADD(FORMAT_B8G8R8A8_UNORM, "b8g8r8a8 unorm");
    ADD(FORMAT_B8G8R8A8_SNORM, "b8g8r8a8 snorm");
    ADD(FORMAT_B8G8R8A8_USCALED, "b8g8r8a8 uscaled");
    ADD(FORMAT_B8G8R8A8_SSCALED, "b8g8r8a8 sscaled");
    ADD(FORMAT_B8G8R8A8_UINT, "b8g8r8a8 uint");
    ADD(FORMAT_B8G8R8A8_SINT, "b8g8r8a8 sint");
    ADD(FORMAT_B8G8R8A8_SRGB, "b8g8r8a8 srgb");
    ADD(FORMAT_A8B8G8R8_UNORM_PACK32, "a8b8g8r8 unorm pack32");
    ADD(FORMAT_A8B8G8R8_SNORM_PACK32, "a8b8g8r8 snorm pack32");
    ADD(FORMAT_A8B8G8R8_USCALED_PACK32, "a8b8g8r8 uscaled pack32");
    ADD(FORMAT_A8B8G8R8_SSCALED_PACK32, "a8b8g8r8 sscaled pack32");
    ADD(FORMAT_A8B8G8R8_UINT_PACK32, "a8b8g8r8 uint pack32");
    ADD(FORMAT_A8B8G8R8_SINT_PACK32, "a8b8g8r8 sint pack32");
    ADD(FORMAT_A8B8G8R8_SRGB_PACK32, "a8b8g8r8 srgb pack32");
    ADD(FORMAT_A2R10G10B10_UNORM_PACK32, "a2r10g10b10 unorm pack32");
    ADD(FORMAT_A2R10G10B10_SNORM_PACK32, "a2r10g10b10 snorm pack32");
    ADD(FORMAT_A2R10G10B10_USCALED_PACK32, "a2r10g10b10 uscaled pack32");
    ADD(FORMAT_A2R10G10B10_SSCALED_PACK32, "a2r10g10b10 sscaled pack32");
    ADD(FORMAT_A2R10G10B10_UINT_PACK32, "a2r10g10b10 uint pack32");
    ADD(FORMAT_A2R10G10B10_SINT_PACK32, "a2r10g10b10 sint pack32");
    ADD(FORMAT_A2B10G10R10_UNORM_PACK32, "a2b10g10r10 unorm pack32");
    ADD(FORMAT_A2B10G10R10_SNORM_PACK32, "a2b10g10r10 snorm pack32");
    ADD(FORMAT_A2B10G10R10_USCALED_PACK32, "a2b10g10r10 uscaled pack32");
    ADD(FORMAT_A2B10G10R10_SSCALED_PACK32, "a2b10g10r10 sscaled pack32");
    ADD(FORMAT_A2B10G10R10_UINT_PACK32, "a2b10g10r10 uint pack32");
    ADD(FORMAT_A2B10G10R10_SINT_PACK32, "a2b10g10r10 sint pack32");
    ADD(FORMAT_R16_UNORM, "r16 unorm");
    ADD(FORMAT_R16_SNORM, "r16 snorm");
    ADD(FORMAT_R16_USCALED, "r16 uscaled");
    ADD(FORMAT_R16_SSCALED, "r16 sscaled");
    ADD(FORMAT_R16_UINT, "r16 uint");
    ADD(FORMAT_R16_SINT, "r16 sint");
    ADD(FORMAT_R16_SFLOAT, "r16 sfloat");
    ADD(FORMAT_R16G16_UNORM, "r16g16 unorm");
    ADD(FORMAT_R16G16_SNORM, "r16g16 snorm");
    ADD(FORMAT_R16G16_USCALED, "r16g16 uscaled");
    ADD(FORMAT_R16G16_SSCALED, "r16g16 sscaled");
    ADD(FORMAT_R16G16_UINT, "r16g16 uint");
    ADD(FORMAT_R16G16_SINT, "r16g16 sint");
    ADD(FORMAT_R16G16_SFLOAT, "r16g16 sfloat");
    ADD(FORMAT_R16G16B16_UNORM, "r16g16b16 unorm");
    ADD(FORMAT_R16G16B16_SNORM, "r16g16b16 snorm");
    ADD(FORMAT_R16G16B16_USCALED, "r16g16b16 uscaled");
    ADD(FORMAT_R16G16B16_SSCALED, "r16g16b16 sscaled");
    ADD(FORMAT_R16G16B16_UINT, "r16g16b16 uint");
    ADD(FORMAT_R16G16B16_SINT, "r16g16b16 sint");
    ADD(FORMAT_R16G16B16_SFLOAT, "r16g16b16 sfloat");
    ADD(FORMAT_R16G16B16A16_UNORM, "r16g16b16a16 unorm");
    ADD(FORMAT_R16G16B16A16_SNORM, "r16g16b16a16 snorm");
    ADD(FORMAT_R16G16B16A16_USCALED, "r16g16b16a16 uscaled");
    ADD(FORMAT_R16G16B16A16_SSCALED, "r16g16b16a16 sscaled");
    ADD(FORMAT_R16G16B16A16_UINT, "r16g16b16a16 uint");
    ADD(FORMAT_R16G16B16A16_SINT, "r16g16b16a16 sint");
    ADD(FORMAT_R16G16B16A16_SFLOAT, "r16g16b16a16 sfloat");
    ADD(FORMAT_R32_UINT, "r32 uint");
    ADD(FORMAT_R32_SINT, "r32 sint");
    ADD(FORMAT_R32_SFLOAT, "r32 sfloat");
    ADD(FORMAT_R32G32_UINT, "r32g32 uint");
    ADD(FORMAT_R32G32_SINT, "r32g32 sint");
    ADD(FORMAT_R32G32_SFLOAT, "r32g32 sfloat");
    ADD(FORMAT_R32G32B32_UINT, "r32g32b32 uint");
    ADD(FORMAT_R32G32B32_SINT, "r32g32b32 sint");
    ADD(FORMAT_R32G32B32_SFLOAT, "r32g32b32 sfloat");
    ADD(FORMAT_R32G32B32A32_UINT, "r32g32b32a32 uint");
    ADD(FORMAT_R32G32B32A32_SINT, "r32g32b32a32 sint");
    ADD(FORMAT_R32G32B32A32_SFLOAT, "r32g32b32a32 sfloat");
    ADD(FORMAT_R64_UINT, "r64 uint");
    ADD(FORMAT_R64_SINT, "r64 sint");
    ADD(FORMAT_R64_SFLOAT, "r64 sfloat");
    ADD(FORMAT_R64G64_UINT, "r64g64 uint");
    ADD(FORMAT_R64G64_SINT, "r64g64 sint");
    ADD(FORMAT_R64G64_SFLOAT, "r64g64 sfloat");
    ADD(FORMAT_R64G64B64_UINT, "r64g64b64 uint");
    ADD(FORMAT_R64G64B64_SINT, "r64g64b64 sint");
    ADD(FORMAT_R64G64B64_SFLOAT, "r64g64b64 sfloat");
    ADD(FORMAT_R64G64B64A64_UINT, "r64g64b64a64 uint");
    ADD(FORMAT_R64G64B64A64_SINT, "r64g64b64a64 sint");
    ADD(FORMAT_R64G64B64A64_SFLOAT, "r64g64b64a64 sfloat");
    ADD(FORMAT_B10G11R11_UFLOAT_PACK32, "b10g11r11 ufloat pack32");
    ADD(FORMAT_E5B9G9R9_UFLOAT_PACK32, "e5b9g9r9 ufloat pack32");
    ADD(FORMAT_D16_UNORM, "d16 unorm");
    ADD(FORMAT_X8_D24_UNORM_PACK32, "x8 d24 unorm pack32");
    ADD(FORMAT_D32_SFLOAT, "d32 sfloat");
    ADD(FORMAT_S8_UINT, "s8 uint");
    ADD(FORMAT_D16_UNORM_S8_UINT, "d16 unorm s8 uint");
    ADD(FORMAT_D24_UNORM_S8_UINT, "d24 unorm s8 uint");
    ADD(FORMAT_D32_SFLOAT_S8_UINT, "d32 sfloat s8 uint");
    ADD(FORMAT_BC1_RGB_UNORM_BLOCK, "bc1 rgb unorm block");
    ADD(FORMAT_BC1_RGB_SRGB_BLOCK, "bc1 rgb srgb block");
    ADD(FORMAT_BC1_RGBA_UNORM_BLOCK, "bc1 rgba unorm block");
    ADD(FORMAT_BC1_RGBA_SRGB_BLOCK, "bc1 rgba srgb block");
    ADD(FORMAT_BC2_UNORM_BLOCK, "bc2 unorm block");
    ADD(FORMAT_BC2_SRGB_BLOCK, "bc2 srgb block");
    ADD(FORMAT_BC3_UNORM_BLOCK, "bc3 unorm block");
    ADD(FORMAT_BC3_SRGB_BLOCK, "bc3 srgb block");
    ADD(FORMAT_BC4_UNORM_BLOCK, "bc4 unorm block");
    ADD(FORMAT_BC4_SNORM_BLOCK, "bc4 snorm block");
    ADD(FORMAT_BC5_UNORM_BLOCK, "bc5 unorm block");
    ADD(FORMAT_BC5_SNORM_BLOCK, "bc5 snorm block");
    ADD(FORMAT_BC6H_UFLOAT_BLOCK, "bc6h ufloat block");
    ADD(FORMAT_BC6H_SFLOAT_BLOCK, "bc6h sfloat block");
    ADD(FORMAT_BC7_UNORM_BLOCK, "bc7 unorm block");
    ADD(FORMAT_BC7_SRGB_BLOCK, "bc7 srgb block");
    ADD(FORMAT_ETC2_R8G8B8_UNORM_BLOCK, "etc2 r8g8b8 unorm block");
    ADD(FORMAT_ETC2_R8G8B8_SRGB_BLOCK, "etc2 r8g8b8 srgb block");
    ADD(FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK, "etc2 r8g8b8a1 unorm block");
    ADD(FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK, "etc2 r8g8b8a1 srgb block");
    ADD(FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK, "etc2 r8g8b8a8 unorm block");
    ADD(FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK, "etc2 r8g8b8a8 srgb block");
    ADD(FORMAT_EAC_R11_UNORM_BLOCK, "eac r11 unorm block");
    ADD(FORMAT_EAC_R11_SNORM_BLOCK, "eac r11 snorm block");
    ADD(FORMAT_EAC_R11G11_UNORM_BLOCK, "eac r11g11 unorm block");
    ADD(FORMAT_EAC_R11G11_SNORM_BLOCK, "eac r11g11 snorm block");
    ADD(FORMAT_ASTC_4x4_UNORM_BLOCK, "astc 4x4 unorm block");
    ADD(FORMAT_ASTC_4x4_SRGB_BLOCK, "astc 4x4 srgb block");
    ADD(FORMAT_ASTC_5x4_UNORM_BLOCK, "astc 5x4 unorm block");
    ADD(FORMAT_ASTC_5x4_SRGB_BLOCK, "astc 5x4 srgb block");
    ADD(FORMAT_ASTC_5x5_UNORM_BLOCK, "astc 5x5 unorm block");
    ADD(FORMAT_ASTC_5x5_SRGB_BLOCK, "astc 5x5 srgb block");
    ADD(FORMAT_ASTC_6x5_UNORM_BLOCK, "astc 6x5 unorm block");
    ADD(FORMAT_ASTC_6x5_SRGB_BLOCK, "astc 6x5 srgb block");
    ADD(FORMAT_ASTC_6x6_UNORM_BLOCK, "astc 6x6 unorm block");
    ADD(FORMAT_ASTC_6x6_SRGB_BLOCK, "astc 6x6 srgb block");
    ADD(FORMAT_ASTC_8x5_UNORM_BLOCK, "astc 8x5 unorm block");
    ADD(FORMAT_ASTC_8x5_SRGB_BLOCK, "astc 8x5 srgb block");
    ADD(FORMAT_ASTC_8x6_UNORM_BLOCK, "astc 8x6 unorm block");
    ADD(FORMAT_ASTC_8x6_SRGB_BLOCK, "astc 8x6 srgb block");
    ADD(FORMAT_ASTC_8x8_UNORM_BLOCK, "astc 8x8 unorm block");
    ADD(FORMAT_ASTC_8x8_SRGB_BLOCK, "astc 8x8 srgb block");
    ADD(FORMAT_ASTC_10x5_UNORM_BLOCK, "astc 10x5 unorm block");
    ADD(FORMAT_ASTC_10x5_SRGB_BLOCK, "astc 10x5 srgb block");
    ADD(FORMAT_ASTC_10x6_UNORM_BLOCK, "astc 10x6 unorm block");
    ADD(FORMAT_ASTC_10x6_SRGB_BLOCK, "astc 10x6 srgb block");
    ADD(FORMAT_ASTC_10x8_UNORM_BLOCK, "astc 10x8 unorm block");
    ADD(FORMAT_ASTC_10x8_SRGB_BLOCK, "astc 10x8 srgb block");
    ADD(FORMAT_ASTC_10x10_UNORM_BLOCK, "astc 10x10 unorm block");
    ADD(FORMAT_ASTC_10x10_SRGB_BLOCK, "astc 10x10 srgb block");
    ADD(FORMAT_ASTC_12x10_UNORM_BLOCK, "astc 12x10 unorm block");
    ADD(FORMAT_ASTC_12x10_SRGB_BLOCK, "astc 12x10 srgb block");
    ADD(FORMAT_ASTC_12x12_UNORM_BLOCK, "astc 12x12 unorm block");
    ADD(FORMAT_ASTC_12x12_SRGB_BLOCK, "astc 12x12 srgb block");
    ADD(FORMAT_G8B8G8R8_422_UNORM, "g8b8g8r8 422 unorm");
    ADD(FORMAT_B8G8R8G8_422_UNORM, "b8g8r8g8 422 unorm");
    ADD(FORMAT_G8_B8_R8_3PLANE_420_UNORM, "g8 b8 r8 3plane 420 unorm");
    ADD(FORMAT_G8_B8R8_2PLANE_420_UNORM, "g8 b8r8 2plane 420 unorm");
    ADD(FORMAT_G8_B8_R8_3PLANE_422_UNORM, "g8 b8 r8 3plane 422 unorm");
    ADD(FORMAT_G8_B8R8_2PLANE_422_UNORM, "g8 b8r8 2plane 422 unorm");
    ADD(FORMAT_G8_B8_R8_3PLANE_444_UNORM, "g8 b8 r8 3plane 444 unorm");
    ADD(FORMAT_R10X6_UNORM_PACK16, "r10x6 unorm pack16");
    ADD(FORMAT_R10X6G10X6_UNORM_2PACK16, "r10x6g10x6 unorm 2pack16");
    ADD(FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16, "r10x6g10x6b10x6a10x6 unorm 4pack16");
    ADD(FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16, "g10x6b10x6g10x6r10x6 422 unorm 4pack16");
    ADD(FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16, "b10x6g10x6r10x6g10x6 422 unorm 4pack16");
    ADD(FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16, "g10x6 b10x6 r10x6 3plane 420 unorm 3pack16");
    ADD(FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16, "g10x6 b10x6r10x6 2plane 420 unorm 3pack16");
    ADD(FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16, "g10x6 b10x6 r10x6 3plane 422 unorm 3pack16");
    ADD(FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16, "g10x6 b10x6r10x6 2plane 422 unorm 3pack16");
    ADD(FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16, "g10x6 b10x6 r10x6 3plane 444 unorm 3pack16");
    ADD(FORMAT_R12X4_UNORM_PACK16, "r12x4 unorm pack16");
    ADD(FORMAT_R12X4G12X4_UNORM_2PACK16, "r12x4g12x4 unorm 2pack16");
    ADD(FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16, "r12x4g12x4b12x4a12x4 unorm 4pack16");
    ADD(FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16, "g12x4b12x4g12x4r12x4 422 unorm 4pack16");
    ADD(FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16, "b12x4g12x4r12x4g12x4 422 unorm 4pack16");
    ADD(FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16, "g12x4 b12x4 r12x4 3plane 420 unorm 3pack16");
    ADD(FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16, "g12x4 b12x4r12x4 2plane 420 unorm 3pack16");
    ADD(FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16, "g12x4 b12x4 r12x4 3plane 422 unorm 3pack16");
    ADD(FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16, "g12x4 b12x4r12x4 2plane 422 unorm 3pack16");
    ADD(FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16, "g12x4 b12x4 r12x4 3plane 444 unorm 3pack16");
    ADD(FORMAT_G16B16G16R16_422_UNORM, "g16b16g16r16 422 unorm");
    ADD(FORMAT_B16G16R16G16_422_UNORM, "b16g16r16g16 422 unorm");
    ADD(FORMAT_G16_B16_R16_3PLANE_420_UNORM, "g16 b16 r16 3plane 420 unorm");
    ADD(FORMAT_G16_B16R16_2PLANE_420_UNORM, "g16 b16r16 2plane 420 unorm");
    ADD(FORMAT_G16_B16_R16_3PLANE_422_UNORM, "g16 b16 r16 3plane 422 unorm");
    ADD(FORMAT_G16_B16R16_2PLANE_422_UNORM, "g16 b16r16 2plane 422 unorm");
    ADD(FORMAT_G16_B16_R16_3PLANE_444_UNORM, "g16 b16 r16 3plane 444 unorm");

    domain = DOMAIN_IMAGE_TYPE; /* VkImageType */
    ADD(IMAGE_TYPE_1D, "1d");
    ADD(IMAGE_TYPE_2D, "2d");
    ADD(IMAGE_TYPE_3D, "3d");

    domain = DOMAIN_IMAGE_TILING; /* VkImageTiling */
    ADD(IMAGE_TILING_OPTIMAL, "optimal");
    ADD(IMAGE_TILING_LINEAR, "linear");
    ADD(IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT, "drm format modifier");

    domain = DOMAIN_PHYSICAL_DEVICE_TYPE; /* VkPhysicalDeviceType */
    ADD(PHYSICAL_DEVICE_TYPE_OTHER, "other");
    ADD(PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, "integrated gpu");
    ADD(PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, "discrete gpu");
    ADD(PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU, "virtual gpu");
    ADD(PHYSICAL_DEVICE_TYPE_CPU, "cpu");

    domain = DOMAIN_QUERY_TYPE; /* VkQueryType */
    ADD(QUERY_TYPE_OCCLUSION, "occlusion");
    ADD(QUERY_TYPE_PIPELINE_STATISTICS, "pipeline statistics");
    ADD(QUERY_TYPE_TIMESTAMP, "timestamp");
    ADD(QUERY_TYPE_TRANSFORM_FEEDBACK_STREAM_EXT, "transform feedback stream");

    domain = DOMAIN_SHARING_MODE; /* VkSharingMode */
    ADD(SHARING_MODE_EXCLUSIVE, "exclusive");
    ADD(SHARING_MODE_CONCURRENT, "concurrent");

    domain = DOMAIN_IMAGE_LAYOUT; /* VkImageLayout */
    ADD(IMAGE_LAYOUT_UNDEFINED, "undefined");
    ADD(IMAGE_LAYOUT_GENERAL, "general");
    ADD(IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, "color attachment optimal");
    ADD(IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, "depth stencil attachment optimal");
    ADD(IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, "depth stencil read only optimal");
    ADD(IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, "shader read only optimal");
    ADD(IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, "transfer src optimal");
    ADD(IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, "transfer dst optimal");
    ADD(IMAGE_LAYOUT_PREINITIALIZED, "preinitialized");
    ADD(IMAGE_LAYOUT_PRESENT_SRC_KHR, "present src");
    ADD(IMAGE_LAYOUT_SHARED_PRESENT_KHR, "shared present");
    ADD(IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL, "depth read only stencil attachment optimal");
    ADD(IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL, "depth attachment stencil read only optimal");

    domain = DOMAIN_IMAGE_VIEW_TYPE; /* VkImageViewType */
    ADD(IMAGE_VIEW_TYPE_1D, "1d");
    ADD(IMAGE_VIEW_TYPE_2D, "2d");
    ADD(IMAGE_VIEW_TYPE_3D, "3d");
    ADD(IMAGE_VIEW_TYPE_CUBE, "cube");
    ADD(IMAGE_VIEW_TYPE_1D_ARRAY, "1d array");
    ADD(IMAGE_VIEW_TYPE_2D_ARRAY, "2d array");
    ADD(IMAGE_VIEW_TYPE_CUBE_ARRAY, "cube array");

    domain = DOMAIN_COMPONENT_SWIZZLE; /* VkComponentSwizzle */
    ADD(COMPONENT_SWIZZLE_IDENTITY, "identity");
    ADD(COMPONENT_SWIZZLE_ZERO, "zero");
    ADD(COMPONENT_SWIZZLE_ONE, "one");
    ADD(COMPONENT_SWIZZLE_R, "r");
    ADD(COMPONENT_SWIZZLE_G, "g");
    ADD(COMPONENT_SWIZZLE_B, "b");
    ADD(COMPONENT_SWIZZLE_A, "a");

    domain = DOMAIN_VERTEX_INPUT_RATE; /* VkVertexInputRate */
    ADD(VERTEX_INPUT_RATE_VERTEX, "vertex");
    ADD(VERTEX_INPUT_RATE_INSTANCE, "instance");

    domain = DOMAIN_PRIMITIVE_TOPOLOGY; /* VkPrimitiveTopology */
    ADD(PRIMITIVE_TOPOLOGY_POINT_LIST, "point list");
    ADD(PRIMITIVE_TOPOLOGY_LINE_LIST, "line list");
    ADD(PRIMITIVE_TOPOLOGY_LINE_STRIP, "line strip");
    ADD(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, "triangle list");
    ADD(PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, "triangle strip");
    ADD(PRIMITIVE_TOPOLOGY_TRIANGLE_FAN, "triangle fan");
    ADD(PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY, "line list with adjacency");
    ADD(PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY, "line strip with adjacency");
    ADD(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY, "triangle list with adjacency");
    ADD(PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY, "triangle strip with adjacency");
    ADD(PRIMITIVE_TOPOLOGY_PATCH_LIST, "patch list");

    domain = DOMAIN_POLYGON_MODE; /* VkPolygonMode */
    ADD(POLYGON_MODE_FILL, "fill");
    ADD(POLYGON_MODE_LINE, "line");
    ADD(POLYGON_MODE_POINT, "point");

    domain = DOMAIN_FRONT_FACE; /* VkFrontFace */
    ADD(FRONT_FACE_COUNTER_CLOCKWISE, "counter clockwise");
    ADD(FRONT_FACE_CLOCKWISE, "clockwise");

    domain = DOMAIN_COMPARE_OP; /* VkCompareOp */
    ADD(COMPARE_OP_NEVER, "never");
    ADD(COMPARE_OP_LESS, "less");
    ADD(COMPARE_OP_EQUAL, "equal");
    ADD(COMPARE_OP_LESS_OR_EQUAL, "less or equal");
    ADD(COMPARE_OP_GREATER, "greater");
    ADD(COMPARE_OP_NOT_EQUAL, "not equal");
    ADD(COMPARE_OP_GREATER_OR_EQUAL, "greater or equal");
    ADD(COMPARE_OP_ALWAYS, "always");

    domain = DOMAIN_STENCIL_OP; /* VkStencilOp */
    ADD(STENCIL_OP_KEEP, "keep");
    ADD(STENCIL_OP_ZERO, "zero");
    ADD(STENCIL_OP_REPLACE, "replace");
    ADD(STENCIL_OP_INCREMENT_AND_CLAMP, "increment and clamp");
    ADD(STENCIL_OP_DECREMENT_AND_CLAMP, "decrement and clamp");
    ADD(STENCIL_OP_INVERT, "invert");
    ADD(STENCIL_OP_INCREMENT_AND_WRAP, "increment and wrap");
    ADD(STENCIL_OP_DECREMENT_AND_WRAP, "decrement and wrap");

    domain = DOMAIN_LOGIC_OP; /* VkLogicOp */
    ADD(LOGIC_OP_CLEAR, "clear");
    ADD(LOGIC_OP_AND, "and");
    ADD(LOGIC_OP_AND_REVERSE, "and reverse");
    ADD(LOGIC_OP_COPY, "copy");
    ADD(LOGIC_OP_AND_INVERTED, "and inverted");
    ADD(LOGIC_OP_NO_OP, "no op");
    ADD(LOGIC_OP_XOR, "xor");
    ADD(LOGIC_OP_OR, "or");
    ADD(LOGIC_OP_NOR, "nor");
    ADD(LOGIC_OP_EQUIVALENT, "equivalent");
    ADD(LOGIC_OP_INVERT, "invert");
    ADD(LOGIC_OP_OR_REVERSE, "or reverse");
    ADD(LOGIC_OP_COPY_INVERTED, "copy inverted");
    ADD(LOGIC_OP_OR_INVERTED, "or inverted");
    ADD(LOGIC_OP_NAND, "nand");
    ADD(LOGIC_OP_SET, "set");

    domain = DOMAIN_BLEND_FACTOR; /* VkBlendFactor */
    ADD(BLEND_FACTOR_ZERO, "zero");
    ADD(BLEND_FACTOR_ONE, "one");
    ADD(BLEND_FACTOR_SRC_COLOR, "src color");
    ADD(BLEND_FACTOR_ONE_MINUS_SRC_COLOR, "one minus src color");
    ADD(BLEND_FACTOR_DST_COLOR, "dst color");
    ADD(BLEND_FACTOR_ONE_MINUS_DST_COLOR, "one minus dst color");
    ADD(BLEND_FACTOR_SRC_ALPHA, "src alpha");
    ADD(BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, "one minus src alpha");
    ADD(BLEND_FACTOR_DST_ALPHA, "dst alpha");
    ADD(BLEND_FACTOR_ONE_MINUS_DST_ALPHA, "one minus dst alpha");
    ADD(BLEND_FACTOR_CONSTANT_COLOR, "constant color");
    ADD(BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR, "one minus constant color");
    ADD(BLEND_FACTOR_CONSTANT_ALPHA, "constant alpha");
    ADD(BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA, "one minus constant alpha");
    ADD(BLEND_FACTOR_SRC_ALPHA_SATURATE, "src alpha saturate");
    ADD(BLEND_FACTOR_SRC1_COLOR, "src1 color");
    ADD(BLEND_FACTOR_ONE_MINUS_SRC1_COLOR, "one minus src1 color");
    ADD(BLEND_FACTOR_SRC1_ALPHA, "src1 alpha");
    ADD(BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA, "one minus src1 alpha");

    domain = DOMAIN_BLEND_OP; /* VkBlendOp */
    ADD(BLEND_OP_ADD, "add");
    ADD(BLEND_OP_SUBTRACT, "subtract");
    ADD(BLEND_OP_REVERSE_SUBTRACT, "reverse subtract");
    ADD(BLEND_OP_MIN, "min");
    ADD(BLEND_OP_MAX, "max");
    ADD(BLEND_OP_ZERO_EXT, "zero");
    ADD(BLEND_OP_SRC_EXT, "src");
    ADD(BLEND_OP_DST_EXT, "dst");
    ADD(BLEND_OP_SRC_OVER_EXT, "src over");
    ADD(BLEND_OP_DST_OVER_EXT, "dst over");
    ADD(BLEND_OP_SRC_IN_EXT, "src in");
    ADD(BLEND_OP_DST_IN_EXT, "dst in");
    ADD(BLEND_OP_SRC_OUT_EXT, "src out");
    ADD(BLEND_OP_DST_OUT_EXT, "dst out");
    ADD(BLEND_OP_SRC_ATOP_EXT, "src atop");
    ADD(BLEND_OP_DST_ATOP_EXT, "dst atop");
    ADD(BLEND_OP_XOR_EXT, "xor");
    ADD(BLEND_OP_MULTIPLY_EXT, "multiply");
    ADD(BLEND_OP_SCREEN_EXT, "screen");
    ADD(BLEND_OP_OVERLAY_EXT, "overlay");
    ADD(BLEND_OP_DARKEN_EXT, "darken");
    ADD(BLEND_OP_LIGHTEN_EXT, "lighten");
    ADD(BLEND_OP_COLORDODGE_EXT, "colordodge");
    ADD(BLEND_OP_COLORBURN_EXT, "colorburn");
    ADD(BLEND_OP_HARDLIGHT_EXT, "hardlight");
    ADD(BLEND_OP_SOFTLIGHT_EXT, "softlight");
    ADD(BLEND_OP_DIFFERENCE_EXT, "difference");
    ADD(BLEND_OP_EXCLUSION_EXT, "exclusion");
    ADD(BLEND_OP_INVERT_EXT, "invert");
    ADD(BLEND_OP_INVERT_RGB_EXT, "invert rgb");
    ADD(BLEND_OP_LINEARDODGE_EXT, "lineardodge");
    ADD(BLEND_OP_LINEARBURN_EXT, "linearburn");
    ADD(BLEND_OP_VIVIDLIGHT_EXT, "vividlight");
    ADD(BLEND_OP_LINEARLIGHT_EXT, "linearlight");
    ADD(BLEND_OP_PINLIGHT_EXT, "pinlight");
    ADD(BLEND_OP_HARDMIX_EXT, "hardmix");
    ADD(BLEND_OP_HSL_HUE_EXT, "hsl hue");
    ADD(BLEND_OP_HSL_SATURATION_EXT, "hsl saturation");
    ADD(BLEND_OP_HSL_COLOR_EXT, "hsl color");
    ADD(BLEND_OP_HSL_LUMINOSITY_EXT, "hsl luminosity");
    ADD(BLEND_OP_PLUS_EXT, "plus");
    ADD(BLEND_OP_PLUS_CLAMPED_EXT, "plus clamped");
    ADD(BLEND_OP_PLUS_CLAMPED_ALPHA_EXT, "plus clamped alpha");
    ADD(BLEND_OP_PLUS_DARKER_EXT, "plus darker");
    ADD(BLEND_OP_MINUS_EXT, "minus");
    ADD(BLEND_OP_MINUS_CLAMPED_EXT, "minus clamped");
    ADD(BLEND_OP_CONTRAST_EXT, "contrast");
    ADD(BLEND_OP_INVERT_OVG_EXT, "invert ovg");
    ADD(BLEND_OP_RED_EXT, "red");
    ADD(BLEND_OP_GREEN_EXT, "green");
    ADD(BLEND_OP_BLUE_EXT, "blue");


    domain = DOMAIN_DYNAMIC_STATE; /* VkDynamicState */
    ADD(DYNAMIC_STATE_VIEWPORT, "viewport");
    ADD(DYNAMIC_STATE_SCISSOR, "scissor");
    ADD(DYNAMIC_STATE_LINE_WIDTH, "line width");
    ADD(DYNAMIC_STATE_DEPTH_BIAS, "depth bias");
    ADD(DYNAMIC_STATE_BLEND_CONSTANTS, "blend constants");
    ADD(DYNAMIC_STATE_DEPTH_BOUNDS, "depth bounds");
    ADD(DYNAMIC_STATE_STENCIL_COMPARE_MASK, "stencil compare mask");
    ADD(DYNAMIC_STATE_STENCIL_WRITE_MASK, "stencil write mask");
    ADD(DYNAMIC_STATE_STENCIL_REFERENCE, "stencil reference");
    ADD(DYNAMIC_STATE_DISCARD_RECTANGLE_EXT, "discard rectangle");
    ADD(DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT, "sample locations");

    domain = DOMAIN_FILTER; /* VkFilter */
    ADD(FILTER_NEAREST, "nearest");
    ADD(FILTER_LINEAR, "linear");

    domain = DOMAIN_SAMPLER_MIPMAP_MODE; /* VkSamplerMipmapMode */
    ADD(SAMPLER_MIPMAP_MODE_NEAREST, "nearest");
    ADD(SAMPLER_MIPMAP_MODE_LINEAR, "linear");

    domain = DOMAIN_SAMPLER_ADDRESS_MODE; /* VkSamplerAddressMode */
    ADD(SAMPLER_ADDRESS_MODE_REPEAT, "repeat");
    ADD(SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT, "mirrored repeat");
    ADD(SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, "clamp to edge");
    ADD(SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, "clamp to border");
    ADD(SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE, "mirror clamp to edge");

    domain = DOMAIN_BORDER_COLOR; /* VkBorderColor */
    ADD(BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, "float transparent black");
    ADD(BORDER_COLOR_INT_TRANSPARENT_BLACK, "int transparent black");
    ADD(BORDER_COLOR_FLOAT_OPAQUE_BLACK, "float opaque black");
    ADD(BORDER_COLOR_INT_OPAQUE_BLACK, "int opaque black");
    ADD(BORDER_COLOR_FLOAT_OPAQUE_WHITE, "float opaque white");
    ADD(BORDER_COLOR_INT_OPAQUE_WHITE, "int opaque white");

    domain = DOMAIN_DESCRIPTOR_TYPE; /* VkDescriptorType */
    ADD(DESCRIPTOR_TYPE_SAMPLER, "sampler");
    ADD(DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, "combined image sampler");
    ADD(DESCRIPTOR_TYPE_SAMPLED_IMAGE, "sampled image");
    ADD(DESCRIPTOR_TYPE_STORAGE_IMAGE, "storage image");
    ADD(DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, "uniform texel buffer");
    ADD(DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, "storage texel buffer");
    ADD(DESCRIPTOR_TYPE_UNIFORM_BUFFER, "uniform buffer");
    ADD(DESCRIPTOR_TYPE_STORAGE_BUFFER, "storage buffer");
    ADD(DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, "uniform buffer dynamic");
    ADD(DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, "storage buffer dynamic");
    ADD(DESCRIPTOR_TYPE_INPUT_ATTACHMENT, "input attachment");
    ADD(DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, "inline uniform block");

    domain = DOMAIN_ATTACHMENT_LOAD_OP; /* VkAttachmentLoadOp */
    ADD(ATTACHMENT_LOAD_OP_LOAD, "load");
    ADD(ATTACHMENT_LOAD_OP_CLEAR, "clear");
    ADD(ATTACHMENT_LOAD_OP_DONT_CARE, "dont care");

    domain = DOMAIN_ATTACHMENT_STORE_OP; /* VkAttachmentStoreOp */
    ADD(ATTACHMENT_STORE_OP_STORE, "store");
    ADD(ATTACHMENT_STORE_OP_DONT_CARE, "dont care");

    domain = DOMAIN_PIPELINE_BIND_POINT; /* VkPipelineBindPoint */
    ADD(PIPELINE_BIND_POINT_GRAPHICS, "graphics");
    ADD(PIPELINE_BIND_POINT_COMPUTE, "compute");

    domain = DOMAIN_COMMAND_BUFFER_LEVEL; /* VkCommandBufferLevel */
    ADD(COMMAND_BUFFER_LEVEL_PRIMARY, "primary");
    ADD(COMMAND_BUFFER_LEVEL_SECONDARY, "secondary");

    domain = DOMAIN_INDEX_TYPE; /* VkIndexType */
    ADD(INDEX_TYPE_UINT16, "uint16");
    ADD(INDEX_TYPE_UINT32, "uint32");

    domain = DOMAIN_SUBPASS_CONTENTS; /* VkSubpassContents */
    ADD(SUBPASS_CONTENTS_INLINE, "inline");
    ADD(SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS, "secondary command buffers");

    domain = DOMAIN_COLOR_SPACE; /* VkColorSpaceKHR */
    ADD(COLOR_SPACE_SRGB_NONLINEAR_KHR, "srgb nonlinear");
    ADD(COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT, "display p3 nonlinear");
    ADD(COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT, "extended srgb linear");
    ADD(COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT, "extended srgb nonlinear");
    ADD(COLOR_SPACE_DCI_P3_LINEAR_EXT, "dci p3 linear");
    ADD(COLOR_SPACE_DCI_P3_NONLINEAR_EXT, "dci p3 nonlinear");
    ADD(COLOR_SPACE_BT709_LINEAR_EXT, "bt709 linear");
    ADD(COLOR_SPACE_BT709_NONLINEAR_EXT, "bt709 nonlinear");
    ADD(COLOR_SPACE_BT2020_LINEAR_EXT, "bt2020 linear");
    ADD(COLOR_SPACE_HDR10_ST2084_EXT, "hdr10 st2084");
    ADD(COLOR_SPACE_DOLBYVISION_EXT, "dolbyvision");
    ADD(COLOR_SPACE_HDR10_HLG_EXT, "hdr10 hlg");
    ADD(COLOR_SPACE_ADOBERGB_LINEAR_EXT, "adobergb linear");
    ADD(COLOR_SPACE_ADOBERGB_NONLINEAR_EXT, "adobergb nonlinear");
    ADD(COLOR_SPACE_PASS_THROUGH_EXT, "pass through");

    domain = DOMAIN_PRESENT_MODE; /* VkPresentModeKHR */
    ADD(PRESENT_MODE_IMMEDIATE_KHR, "immediate");
    ADD(PRESENT_MODE_MAILBOX_KHR, "mailbox");
    ADD(PRESENT_MODE_FIFO_KHR, "fifo");
    ADD(PRESENT_MODE_FIFO_RELAXED_KHR, "fifo relaxed");
    ADD(PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR, "shared demand refresh");
    ADD(PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR, "shared continuous refresh");

    domain = DOMAIN_DEBUG_REPORT_OBJECT_TYPE; /* VkDebugReportObjectTypeEXT */
    ADD(DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT, "unknown");
    ADD(DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT, "instance");
    ADD(DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT, "physical device");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT, "device");
    ADD(DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT, "queue");
    ADD(DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT, "semaphore");
    ADD(DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT, "command buffer");
    ADD(DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT, "fence");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, "device memory");
    ADD(DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, "buffer");
    ADD(DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, "image");
    ADD(DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT, "event");
    ADD(DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT, "query pool");
    ADD(DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT, "buffer view");
    ADD(DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, "image view");
    ADD(DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, "shader module");
    ADD(DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT, "pipeline cache");
    ADD(DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT, "pipeline layout");
    ADD(DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, "render pass");
    ADD(DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, "pipeline");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT, "descriptor set layout");
    ADD(DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT, "sampler");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT, "descriptor pool");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, "descriptor set");
    ADD(DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT, "framebuffer");
    ADD(DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT, "command pool");
    ADD(DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT, "surface");
    ADD(DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT, "swapchain");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT, "debug report callback");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT, "display");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT, "display mode");
    ADD(DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT, "validation cache");
    ADD(DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_EXT, "sampler ycbcr conversion");
    ADD(DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_EXT , "descriptor update template");

    domain = DOMAIN_DESCRIPTOR_UPDATE_TEMPLATE_TYPE; /* VkDescriptorUpdateTemplateType */
    ADD(DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET, "descriptor set");
    ADD(DESCRIPTOR_UPDATE_TEMPLATE_TYPE_PUSH_DESCRIPTORS_KHR, "push descriptors");

    domain = DOMAIN_VALIDATION_CHECK; /* VkValidationCheckEXT */
    ADD(VALIDATION_CHECK_ALL_EXT, "all");
    ADD(VALIDATION_CHECK_SHADERS_EXT, "shaders");

    domain = DOMAIN_DISPLAY_POWER_STATE; /* VkDisplayPowerStateEXT */
    ADD(DISPLAY_POWER_STATE_OFF_EXT, "off");
    ADD(DISPLAY_POWER_STATE_SUSPEND_EXT, "suspend");
    ADD(DISPLAY_POWER_STATE_ON_EXT, "on");

    domain = DOMAIN_DEVICE_EVENT_TYPE; /* VkDeviceEventTypeEXT */
    ADD(DEVICE_EVENT_TYPE_DISPLAY_HOTPLUG_EXT, "display hotplug");

    domain = DOMAIN_DISPLAY_EVENT_TYPE; /* VkDisplayEventTypeEXT */
    ADD(DISPLAY_EVENT_TYPE_FIRST_PIXEL_OUT_EXT, "first pixel out");

    domain = DOMAIN_OBJECT_TYPE; /* VkObjectType */
    ADD(OBJECT_TYPE_UNKNOWN, "unknown");
    ADD(OBJECT_TYPE_INSTANCE, "instance");
    ADD(OBJECT_TYPE_PHYSICAL_DEVICE, "physical device");
    ADD(OBJECT_TYPE_DEVICE, "device");
    ADD(OBJECT_TYPE_QUEUE, "queue");
    ADD(OBJECT_TYPE_SEMAPHORE, "semaphore");
    ADD(OBJECT_TYPE_COMMAND_BUFFER, "command buffer");
    ADD(OBJECT_TYPE_FENCE, "fence");
    ADD(OBJECT_TYPE_DEVICE_MEMORY, "device memory");
    ADD(OBJECT_TYPE_BUFFER, "buffer");
    ADD(OBJECT_TYPE_IMAGE, "image");
    ADD(OBJECT_TYPE_EVENT, "event");
    ADD(OBJECT_TYPE_QUERY_POOL, "query pool");
    ADD(OBJECT_TYPE_BUFFER_VIEW, "buffer view");
    ADD(OBJECT_TYPE_IMAGE_VIEW, "image view");
    ADD(OBJECT_TYPE_SHADER_MODULE, "shader module");
    ADD(OBJECT_TYPE_PIPELINE_CACHE, "pipeline cache");
    ADD(OBJECT_TYPE_PIPELINE_LAYOUT, "pipeline layout");
    ADD(OBJECT_TYPE_RENDER_PASS, "render pass");
    ADD(OBJECT_TYPE_PIPELINE, "pipeline");
    ADD(OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, "descriptor set layout");
    ADD(OBJECT_TYPE_SAMPLER, "sampler");
    ADD(OBJECT_TYPE_DESCRIPTOR_POOL, "descriptor pool");
    ADD(OBJECT_TYPE_DESCRIPTOR_SET, "descriptor set");
    ADD(OBJECT_TYPE_FRAMEBUFFER, "framebuffer");
    ADD(OBJECT_TYPE_COMMAND_POOL, "command pool");
    ADD(OBJECT_TYPE_SURFACE_KHR, "surface");
    ADD(OBJECT_TYPE_SWAPCHAIN_KHR, "swapchain");
    ADD(OBJECT_TYPE_DISPLAY_KHR, "display");
    ADD(OBJECT_TYPE_DISPLAY_MODE_KHR, "display mode");
    ADD(OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT, "debug report callback");
    ADD(OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE, "descriptor update template");
    ADD(OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION, "sampler ycbcr conversion");
    ADD(OBJECT_TYPE_VALIDATION_CACHE_EXT, "validation cache");
    ADD(OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT, "debug utils messenger");

    domain = DOMAIN_BLEND_OVERLAP; /* VkBlendOverlapEXT */
    ADD(BLEND_OVERLAP_UNCORRELATED_EXT, "uncorrelated");
    ADD(BLEND_OVERLAP_DISJOINT_EXT, "disjoint");
    ADD(BLEND_OVERLAP_CONJOINT_EXT, "conjoint");

    domain = DOMAIN_SAMPLER_REDUCTION_MODE; /* VkSamplerReductionModeEXT */
    ADD(SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT, "weighted average");
    ADD(SAMPLER_REDUCTION_MODE_MIN_EXT, "min");
    ADD(SAMPLER_REDUCTION_MODE_MAX_EXT, "max");

    domain = DOMAIN_DISCARD_RECTANGLE_MODE; /* VkDiscardRectangleModeEXT */
    ADD(DISCARD_RECTANGLE_MODE_INCLUSIVE_EXT, "inclusive");
    ADD(DISCARD_RECTANGLE_MODE_EXCLUSIVE_EXT, "exclusive");

    domain = DOMAIN_POINT_CLIPPING_BEHAVIOR; /* VkPointClippingBehavior */
    ADD(POINT_CLIPPING_BEHAVIOR_ALL_CLIP_PLANES, "all clip planes");
    ADD(POINT_CLIPPING_BEHAVIOR_USER_CLIP_PLANES_ONLY, "user clip planes only");

    domain = DOMAIN_TESSELLATION_DOMAIN_ORIGIN; /* VkTessellationDomainOrigin */
    ADD(TESSELLATION_DOMAIN_ORIGIN_UPPER_LEFT, "upper left");
    ADD(TESSELLATION_DOMAIN_ORIGIN_LOWER_LEFT, "lower left");

    domain = DOMAIN_SAMPLER_YCBCR_MODEL_CONVERSION; /* VkSamplerYcbcrModelConversion */
    ADD(SAMPLER_YCBCR_MODEL_CONVERSION_RGB_IDENTITY, "rgb identity");
    ADD(SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_IDENTITY, "ycbcr identity");
    ADD(SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_709, "ycbcr 709");
    ADD(SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_601, "ycbcr 601");
    ADD(SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_2020, "ycbcr 2020");

    domain = DOMAIN_SAMPLER_YCBCR_RANGE; /* VkSamplerYcbcrRange */
    ADD(SAMPLER_YCBCR_RANGE_ITU_FULL, "itu full");
    ADD(SAMPLER_YCBCR_RANGE_ITU_NARROW, "itu narrow");

    domain = DOMAIN_CHROMA_LOCATION; /* VkChromaLocation */
    ADD(CHROMA_LOCATION_COSITED_EVEN, "cosited even");
    ADD(CHROMA_LOCATION_MIDPOINT, "midpoint");

    domain = DOMAIN_VALIDATION_CACHE_HEADER_VERSION; /* VkValidationCacheHeaderVersionKHR */
    ADD(VALIDATION_CACHE_HEADER_VERSION_ONE_EXT, "one");

    domain = DOMAIN_QUEUE_GLOBAL_PRIORITY; /* VkQueueGlobalPriorityEXT */
    ADD(QUEUE_GLOBAL_PRIORITY_LOW_EXT, "low");
    ADD(QUEUE_GLOBAL_PRIORITY_MEDIUM_EXT, "medium");
    ADD(QUEUE_GLOBAL_PRIORITY_HIGH_EXT, "high");
    ADD(QUEUE_GLOBAL_PRIORITY_REALTIME_EXT, "realtime");

    domain = DOMAIN_CONSERVATIVE_RASTERIZATION_MODE; /* VkConservativeRasterizationModeEXT */
    ADD(CONSERVATIVE_RASTERIZATION_MODE_DISABLED_EXT, "disabled");
    ADD(CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT, "overestimate");
    ADD(CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT, "underestimate");

    domain = DOMAIN_VENDOR_ID; /* VkVendorId */
    ADD(VENDOR_ID_VIV, "viv");
    ADD(VENDOR_ID_VSI, "vsi");
    ADD(VENDOR_ID_KAZAN, "kazan");

    domain = DOMAIN_DRIVER_ID; /* VkDriverIdKHR */
    ADD(DRIVER_ID_AMD_PROPRIETARY_KHR, "amd proprietary");
    ADD(DRIVER_ID_AMD_OPEN_SOURCE_KHR, "amd open source");
    ADD(DRIVER_ID_MESA_RADV_KHR, "mesa radv");
    ADD(DRIVER_ID_NVIDIA_PROPRIETARY_KHR, "nvidia proprietary");
    ADD(DRIVER_ID_INTEL_PROPRIETARY_WINDOWS_KHR, "intel proprietary windows");
    ADD(DRIVER_ID_INTEL_OPEN_SOURCE_MESA_KHR, "intel open source mesa");
    ADD(DRIVER_ID_IMAGINATION_PROPRIETARY_KHR, "imagination proprietary");
    ADD(DRIVER_ID_QUALCOMM_PROPRIETARY_KHR, "qualcomm proprietary");
    ADD(DRIVER_ID_ARM_PROPRIETARY_KHR, "arm proprietary");

    domain = DOMAIN_TIME_DOMAIN; /* VkTimaDomainEXT */
    ADD(TIME_DOMAIN_DEVICE_EXT, "device");
    ADD(TIME_DOMAIN_CLOCK_MONOTONIC_EXT, "clock monotonic");
    ADD(TIME_DOMAIN_CLOCK_MONOTONIC_RAW_EXT, "clock monotonic raw");
    ADD(TIME_DOMAIN_QUERY_PERFORMANCE_COUNTER_EXT, "query performance counter");

#undef ADD
    }

