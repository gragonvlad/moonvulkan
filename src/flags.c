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

#define ADD(c) do { lua_pushinteger(L, VK_##c); lua_setfield(L, -2, #c); } while(0)

/* checkkkflags: accepts a list of strings starting from index=arg
 * pushxxxflags -> pushes a list of strings 
 */
/*----------------------------------------------------------------------*
 | Reserved
 *----------------------------------------------------------------------*/

static VkFlags checkreservedflags(lua_State *L, int arg)
    {
    const char *s;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        }

    return 0;
    }

static int pushreservedflags(lua_State *L, VkFlags flags)
    {
    (void)L; (void)flags;
    return 0;
    }


static int ReservedFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushreservedflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkreservedflags(L, 1));
    return 1;
    }


/*----------------------------------------------------------------------*
 | VkStencilFaceFlags
 *----------------------------------------------------------------------*/

static VkFlags checkstencilfaceflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_STENCIL_FACE_FRONT_BIT, "front");
    CASE(VK_STENCIL_FACE_BACK_BIT, "back");
//  CASE(VK_STENCIL_FRONT_AND_BACK, "front and back");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushstencilfaceflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_STENCIL_FACE_FRONT_BIT, "front");
    CASE(VK_STENCIL_FACE_BACK_BIT, "back");
//  CASE(VK_STENCIL_FRONT_AND_BACK, "front and back");
#undef CASE

    return n;
    }

static int StencilFaceFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushstencilfaceflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkstencilfaceflags(L, 1));
    return 1;
    }

#define Add_StencilFaceFlags(L) \
    ADD(STENCIL_FACE_FRONT_BIT);\
    ADD(STENCIL_FACE_BACK_BIT);\
    ADD(STENCIL_FRONT_AND_BACK);\



/*----------------------------------------------------------------------*
 | VkCommandBufferResetFlags
 *----------------------------------------------------------------------*/

static VkFlags checkcommandbufferresetflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT, "release resources");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushcommandbufferresetflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT, "release resources");
#undef CASE

    return n;
    }

static int CommandBufferResetFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushcommandbufferresetflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkcommandbufferresetflags(L, 1));
    return 1;
    }

#define Add_CommandBufferResetFlags(L)  \
    ADD(COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);\



/*----------------------------------------------------------------------*
 | VkQueryControlFlags
 *----------------------------------------------------------------------*/

static VkFlags checkquerycontrolflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_QUERY_CONTROL_PRECISE_BIT, "precise");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushquerycontrolflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_QUERY_CONTROL_PRECISE_BIT, "precise");
#undef CASE

    return n;
    }

static int QueryControlFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushquerycontrolflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkquerycontrolflags(L, 1));
    return 1;
    }

#define Add_QueryControlFlags(L)    \
    ADD(QUERY_CONTROL_PRECISE_BIT);\


/*----------------------------------------------------------------------*
 | VkCommandBufferUsageFlags
 *----------------------------------------------------------------------*/

static VkFlags checkcommandbufferusageflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, "one time submit");
    CASE(VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, "render pass continue");
    CASE(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, "simultaneous use");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushcommandbufferusageflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, "one time submit");
    CASE(VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, "render pass continue");
    CASE(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, "simultaneous use");
#undef CASE

    return n;
    }

static int CommandBufferUsageFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushcommandbufferusageflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkcommandbufferusageflags(L, 1));
    return 1;
    }

#define Add_CommandBufferUsageFlags(L)  \
    ADD(COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);\
    ADD(COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);\
    ADD(COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);\


/*----------------------------------------------------------------------*
 | VkCommandPoolResetFlags
 *----------------------------------------------------------------------*/

static VkFlags checkcommandpoolresetflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT, "release resources");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushcommandpoolresetflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT, "release resources");
#undef CASE

    return n;
    }

static int CommandPoolResetFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushcommandpoolresetflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkcommandpoolresetflags(L, 1));
    return 1;
    }

#define Add_CommandPoolResetFlags(L)    \
    ADD(COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);\



/*----------------------------------------------------------------------*
 | VkCommandPoolCreateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkcommandpoolcreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, "transient");
    CASE(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, "reset command buffer");
    CASE(VK_COMMAND_POOL_CREATE_PROTECTED_BIT, "protected");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushcommandpoolcreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, "transient");
    CASE(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, "reset command buffer");
    CASE(VK_COMMAND_POOL_CREATE_PROTECTED_BIT, "protected");
#undef CASE

    return n;
    }

static int CommandPoolCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushcommandpoolcreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkcommandpoolcreateflags(L, 1));
    return 1;
    }

#define Add_CommandPoolCreateFlags(L)   \
    ADD(COMMAND_POOL_CREATE_TRANSIENT_BIT);\
    ADD(COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);\
    ADD(COMMAND_POOL_CREATE_PROTECTED_BIT);\

/*----------------------------------------------------------------------*
 | VkDependencyFlags
 *----------------------------------------------------------------------*/

static VkFlags checkdependencyflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_DEPENDENCY_BY_REGION_BIT, "by region");
    CASE(VK_DEPENDENCY_DEVICE_GROUP_BIT, "device group");
    CASE(VK_DEPENDENCY_VIEW_LOCAL_BIT, "view local");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdependencyflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_DEPENDENCY_BY_REGION_BIT, "by region");
    CASE(VK_DEPENDENCY_DEVICE_GROUP_BIT, "device group");
    CASE(VK_DEPENDENCY_VIEW_LOCAL_BIT, "view local");
#undef CASE

    return n;
    }

static int DependencyFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdependencyflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdependencyflags(L, 1));
    return 1;
    }

#define Add_DependencyFlags(L)  \
    ADD(DEPENDENCY_BY_REGION_BIT);\
    ADD(DEPENDENCY_DEVICE_GROUP_BIT);\
    ADD(DEPENDENCY_VIEW_LOCAL_BIT);\


/*----------------------------------------------------------------------*
 | VkAccessFlags
 *----------------------------------------------------------------------*/

static VkFlags checkaccessflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_ACCESS_INDIRECT_COMMAND_READ_BIT, "indirect command read");
    CASE(VK_ACCESS_INDEX_READ_BIT, "index read");
    CASE(VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, "vertex attribute read");
    CASE(VK_ACCESS_UNIFORM_READ_BIT, "uniform read");
    CASE(VK_ACCESS_INPUT_ATTACHMENT_READ_BIT, "input attachment read");
    CASE(VK_ACCESS_SHADER_READ_BIT, "shader read");
    CASE(VK_ACCESS_SHADER_WRITE_BIT, "shader write");
    CASE(VK_ACCESS_COLOR_ATTACHMENT_READ_BIT, "color attachment read");
    CASE(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, "color attachment write");
    CASE(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT, "depth stencil attachment read");
    CASE(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, "depth stencil attachment write");
    CASE(VK_ACCESS_TRANSFER_READ_BIT, "transfer read");
    CASE(VK_ACCESS_TRANSFER_WRITE_BIT, "transfer write");
    CASE(VK_ACCESS_HOST_READ_BIT, "host read");
    CASE(VK_ACCESS_HOST_WRITE_BIT, "host write");
    CASE(VK_ACCESS_MEMORY_READ_BIT, "memory read");
    CASE(VK_ACCESS_MEMORY_WRITE_BIT, "memory write");
    CASE(VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT, "color attachment read noncoherent");
    CASE(VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT, "conditional rendering read");
    CASE(VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT, "transform feedback write");
    CASE(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT, "transform feedback counter read");
    CASE(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, "transform feedback counter write");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushaccessflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_ACCESS_INDIRECT_COMMAND_READ_BIT, "indirect command read");
    CASE(VK_ACCESS_INDEX_READ_BIT, "index read");
    CASE(VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, "vertex attribute read");
    CASE(VK_ACCESS_UNIFORM_READ_BIT, "uniform read");
    CASE(VK_ACCESS_INPUT_ATTACHMENT_READ_BIT, "input attachment read");
    CASE(VK_ACCESS_SHADER_READ_BIT, "shader read");
    CASE(VK_ACCESS_SHADER_WRITE_BIT, "shader write");
    CASE(VK_ACCESS_COLOR_ATTACHMENT_READ_BIT, "color attachment read");
    CASE(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, "color attachment write");
    CASE(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT, "depth stencil attachment read");
    CASE(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, "depth stencil attachment write");
    CASE(VK_ACCESS_TRANSFER_READ_BIT, "transfer read");
    CASE(VK_ACCESS_TRANSFER_WRITE_BIT, "transfer write");
    CASE(VK_ACCESS_HOST_READ_BIT, "host read");
    CASE(VK_ACCESS_HOST_WRITE_BIT, "host write");
    CASE(VK_ACCESS_MEMORY_READ_BIT, "memory read");
    CASE(VK_ACCESS_MEMORY_WRITE_BIT, "memory write");
    CASE(VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT, "color attachment read noncoherent");
    CASE(VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT, "conditional rendering read");
    CASE(VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT, "transform feedback write");
    CASE(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT, "transform feedback counter read");
    CASE(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, "transform feedback counter write");
#undef CASE
    return n;
    }

static int AccessFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushaccessflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkaccessflags(L, 1));
    return 1;
    }

#define Add_AccessFlags(L)  \
    ADD(ACCESS_INDIRECT_COMMAND_READ_BIT);\
    ADD(ACCESS_INDEX_READ_BIT);\
    ADD(ACCESS_VERTEX_ATTRIBUTE_READ_BIT);\
    ADD(ACCESS_UNIFORM_READ_BIT);\
    ADD(ACCESS_INPUT_ATTACHMENT_READ_BIT);\
    ADD(ACCESS_SHADER_READ_BIT);\
    ADD(ACCESS_SHADER_WRITE_BIT);\
    ADD(ACCESS_COLOR_ATTACHMENT_READ_BIT);\
    ADD(ACCESS_COLOR_ATTACHMENT_WRITE_BIT);\
    ADD(ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);\
    ADD(ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);\
    ADD(ACCESS_TRANSFER_READ_BIT);\
    ADD(ACCESS_TRANSFER_WRITE_BIT);\
    ADD(ACCESS_HOST_READ_BIT);\
    ADD(ACCESS_HOST_WRITE_BIT);\
    ADD(ACCESS_MEMORY_READ_BIT);\
    ADD(ACCESS_MEMORY_WRITE_BIT);\
    ADD(ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT);\
    ADD(ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT);\
    ADD(ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT);\
    ADD(ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);\
    ADD(ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT);\



/*----------------------------------------------------------------------*
 | VkAttachmentDescriptionFlags
 *----------------------------------------------------------------------*/

static VkFlags checkattachmentdescriptionflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT, "may alias");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushattachmentdescriptionflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT, "may alias");
#undef CASE

    return n;
    }

static int AttachmentDescriptionFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushattachmentdescriptionflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkattachmentdescriptionflags(L, 1));
    return 1;
    }

#define Add_AttachmentDescriptionFlags(L)   \
    ADD(ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT);\

/*----------------------------------------------------------------------*
 | VkDescriptorPoolCreateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkdescriptorpoolcreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, "free descriptor set");
    CASE(VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT, "update after bind");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdescriptorpoolcreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT, "free descriptor set");
#undef CASE

    return n;
    }

static int DescriptorPoolCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdescriptorpoolcreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdescriptorpoolcreateflags(L, 1));
    return 1;
    }

#define Add_DescriptorPoolCreateFlags(L)    \
    ADD(DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);\


/*----------------------------------------------------------------------*
 | VkDescriptorSetLayoutCreateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkdescriptorsetlayoutcreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR, "push descriptor");
        CASE(VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT, "update after bind pool");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdescriptorsetlayoutcreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR, "push descriptor");
#undef CASE

    return n;
    }

static int DescriptorSetLayoutCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdescriptorsetlayoutcreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdescriptorsetlayoutcreateflags(L, 1));
    return 1;
    }

#define Add_DescriptorSetLayoutCreateFlags(L)   \
    ADD(DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR);\

/*----------------------------------------------------------------------*
 | VkColorComponentFlags
 *----------------------------------------------------------------------*/

static VkFlags checkcolorcomponentflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_COLOR_COMPONENT_R_BIT, "r");
    CASE(VK_COLOR_COMPONENT_G_BIT, "g");
    CASE(VK_COLOR_COMPONENT_B_BIT, "b");
    CASE(VK_COLOR_COMPONENT_A_BIT, "a");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushcolorcomponentflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_COLOR_COMPONENT_R_BIT, "r");
    CASE(VK_COLOR_COMPONENT_G_BIT, "g");
    CASE(VK_COLOR_COMPONENT_B_BIT, "b");
    CASE(VK_COLOR_COMPONENT_A_BIT, "a");
#undef CASE

    return n;
    }

static int ColorComponentFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushcolorcomponentflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkcolorcomponentflags(L, 1));
    return 1;
    }

#define Add_ColorComponentFlags(L)  \
    ADD(COLOR_COMPONENT_R_BIT);\
    ADD(COLOR_COMPONENT_G_BIT);\
    ADD(COLOR_COMPONENT_B_BIT);\
    ADD(COLOR_COMPONENT_A_BIT);\



/*----------------------------------------------------------------------*
 | VkCullModeFlags
 *----------------------------------------------------------------------*/

static VkFlags checkcullmodeflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_CULL_MODE_NONE, "none");
    CASE(VK_CULL_MODE_FRONT_BIT, "front");
    CASE(VK_CULL_MODE_BACK_BIT, "back");
    CASE(VK_CULL_MODE_FRONT_AND_BACK, "front and back");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushcullmodeflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

//    if(flags==0) { lua_pushstring(L, "none"); return 1; }

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
//  CASE(VK_CULL_MODE_NONE, "none");
    CASE(VK_CULL_MODE_FRONT_BIT, "front");
    CASE(VK_CULL_MODE_BACK_BIT, "back");
//  CASE(VK_CULL_MODE_FRONT_AND_BACK, "front and back");
#undef CASE

    return n;
    }

static int CullModeFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushcullmodeflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkcullmodeflags(L, 1));
    return 1;
    }

#define Add_CullModeFlags(L)    \
    ADD(CULL_MODE_NONE);\
    ADD(CULL_MODE_FRONT_BIT);\
    ADD(CULL_MODE_BACK_BIT);\
    ADD(CULL_MODE_FRONT_AND_BACK);\



/*----------------------------------------------------------------------*
 | VkShaderStageFlags
 *----------------------------------------------------------------------*/

static VkFlags checkshaderstageflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_SHADER_STAGE_VERTEX_BIT, "vertex");
    CASE(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, "tessellation control");
    CASE(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, "tessellation evaluation");
    CASE(VK_SHADER_STAGE_GEOMETRY_BIT, "geometry");
    CASE(VK_SHADER_STAGE_FRAGMENT_BIT, "fragment");
    CASE(VK_SHADER_STAGE_COMPUTE_BIT, "compute");
    // These are not individual bits:
    CASE(VK_SHADER_STAGE_ALL_GRAPHICS, "all graphics");
    CASE(VK_SHADER_STAGE_ALL, "all");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushshaderstageflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_SHADER_STAGE_VERTEX_BIT, "vertex");
    CASE(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, "tessellation control");
    CASE(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, "tessellation evaluation");
    CASE(VK_SHADER_STAGE_GEOMETRY_BIT, "geometry");
    CASE(VK_SHADER_STAGE_FRAGMENT_BIT, "fragment");
    CASE(VK_SHADER_STAGE_COMPUTE_BIT, "compute");
    // These are not individual bits:
    CASE(VK_SHADER_STAGE_ALL_GRAPHICS, "all graphics"); 
    CASE(VK_SHADER_STAGE_ALL, "all");
#undef CASE

    return n;
    }

static int ShaderStageFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushshaderstageflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkshaderstageflags(L, 1));
    return 1;
    }

#define Add_ShaderStageFlags(L) \
    ADD(SHADER_STAGE_VERTEX_BIT);\
    ADD(SHADER_STAGE_TESSELLATION_CONTROL_BIT);\
    ADD(SHADER_STAGE_TESSELLATION_EVALUATION_BIT);\
    ADD(SHADER_STAGE_GEOMETRY_BIT);\
    ADD(SHADER_STAGE_FRAGMENT_BIT);\
    ADD(SHADER_STAGE_COMPUTE_BIT);\
    ADD(SHADER_STAGE_ALL_GRAPHICS);\
    ADD(SHADER_STAGE_ALL);\



/*----------------------------------------------------------------------*
 | VkPipelineCreateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkpipelinecreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT, "disable optimization");
    CASE(VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT, "allow derivatives");
    CASE(VK_PIPELINE_CREATE_DERIVATIVE_BIT, "derivative");
    CASE(VK_PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT, "view index from device index");
    CASE(VK_PIPELINE_CREATE_DISPATCH_BASE, "dispatch base");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushpipelinecreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT, "disable optimization");
    CASE(VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT, "allow derivatives");
    CASE(VK_PIPELINE_CREATE_DERIVATIVE_BIT, "derivative");
    CASE(VK_PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT, "view index from device index");
    CASE(VK_PIPELINE_CREATE_DISPATCH_BASE, "dispatch base");
#undef CASE

    return n;
    }

static int PipelineCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushpipelinecreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkpipelinecreateflags(L, 1));
    return 1;
    }

#define VK_PIPELINE_CREATE_DISPATCH_BASE_BIT VK_PIPELINE_CREATE_DISPATCH_BASE /*@@ spec bug ? */
#define Add_PipelineCreateFlags(L)  \
    ADD(PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT);\
    ADD(PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT);\
    ADD(PIPELINE_CREATE_DERIVATIVE_BIT);\
    ADD(PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT);\
    ADD(PIPELINE_CREATE_DISPATCH_BASE_BIT);\


/*----------------------------------------------------------------------*
 | VkBufferUsageFlags
 *----------------------------------------------------------------------*/

static VkFlags checkbufferusageflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, "transfer src");
    CASE(VK_BUFFER_USAGE_TRANSFER_DST_BIT, "transfer dst");
    CASE(VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT, "uniform texel buffer");
    CASE(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, "storage texel buffer");
    CASE(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, "uniform buffer");
    CASE(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, "storage buffer");
    CASE(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, "index buffer");
    CASE(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, "vertex buffer");
    CASE(VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, "indirect buffer");
    CASE(VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT, "conditional rendering");
    CASE(VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT, "transform feedback buffer");
    CASE(VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT, "transform feedback counter buffer");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushbufferusageflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, "transfer src");
    CASE(VK_BUFFER_USAGE_TRANSFER_DST_BIT, "transfer dst");
    CASE(VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT, "uniform texel buffer");
    CASE(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, "storage texel buffer");
    CASE(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, "uniform buffer");
    CASE(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, "storage buffer");
    CASE(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, "index buffer");
    CASE(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, "vertex buffer");
    CASE(VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, "indirect buffer");
    CASE(VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT, "conditional rendering");
    CASE(VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT, "transform feedback buffer");
    CASE(VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT, "transform feedback counter buffer");
#undef CASE

    return n;
    }

static int BufferUsageFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushbufferusageflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkbufferusageflags(L, 1));
    return 1;
    }

#define Add_BufferUsageFlags(L) \
    ADD(BUFFER_USAGE_TRANSFER_SRC_BIT);\
    ADD(BUFFER_USAGE_TRANSFER_DST_BIT);\
    ADD(BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);\
    ADD(BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);\
    ADD(BUFFER_USAGE_UNIFORM_BUFFER_BIT);\
    ADD(BUFFER_USAGE_STORAGE_BUFFER_BIT);\
    ADD(BUFFER_USAGE_INDEX_BUFFER_BIT);\
    ADD(BUFFER_USAGE_VERTEX_BUFFER_BIT);\
    ADD(BUFFER_USAGE_INDIRECT_BUFFER_BIT);\
    ADD(BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT);\
    ADD(BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT);\
    ADD(BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT);\


/*----------------------------------------------------------------------*
 | VkBufferCreateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkbuffercreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_BUFFER_CREATE_SPARSE_BINDING_BIT, "sparse binding");
    CASE(VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT, "sparse residency");
    CASE(VK_BUFFER_CREATE_SPARSE_ALIASED_BIT, "sparse aliased");
    CASE(VK_BUFFER_CREATE_PROTECTED_BIT, "protected");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushbuffercreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_BUFFER_CREATE_SPARSE_BINDING_BIT, "sparse binding");
    CASE(VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT, "sparse residency");
    CASE(VK_BUFFER_CREATE_SPARSE_ALIASED_BIT, "sparse aliased");
    CASE(VK_BUFFER_CREATE_PROTECTED_BIT, "protected");
#undef CASE

    return n;
    }

static int BufferCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushbuffercreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkbuffercreateflags(L, 1));
    return 1;
    }

#define Add_BufferCreateFlags(L)    \
    ADD(BUFFER_CREATE_SPARSE_BINDING_BIT);\
    ADD(BUFFER_CREATE_SPARSE_RESIDENCY_BIT);\
    ADD(BUFFER_CREATE_SPARSE_ALIASED_BIT);\
    ADD(BUFFER_CREATE_PROTECTED_BIT);\


/*----------------------------------------------------------------------*
 | VkQueryResultFlags
 *----------------------------------------------------------------------*/

static VkFlags checkqueryresultflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_QUERY_RESULT_64_BIT, "64");
    CASE(VK_QUERY_RESULT_WAIT_BIT, "wait");
    CASE(VK_QUERY_RESULT_WITH_AVAILABILITY_BIT, "with availability");
    CASE(VK_QUERY_RESULT_PARTIAL_BIT, "partial");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushqueryresultflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_QUERY_RESULT_64_BIT, "64");
    CASE(VK_QUERY_RESULT_WAIT_BIT, "wait");
    CASE(VK_QUERY_RESULT_WITH_AVAILABILITY_BIT, "with availability");
    CASE(VK_QUERY_RESULT_PARTIAL_BIT, "partial");
#undef CASE

    return n;
    }

static int QueryResultFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushqueryresultflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkqueryresultflags(L, 1));
    return 1;
    }

#define Add_QueryResultFlags(L) \
    ADD(QUERY_RESULT_64_BIT);\
    ADD(QUERY_RESULT_WAIT_BIT);\
    ADD(QUERY_RESULT_WITH_AVAILABILITY_BIT);\
    ADD(QUERY_RESULT_PARTIAL_BIT);\



/*----------------------------------------------------------------------*
 | VkQueryPipelineStatisticFlags
 *----------------------------------------------------------------------*/

static VkFlags checkquerypipelinestatisticflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT, "input assembly vertices");
    CASE(VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT, "input assembly primitives");
    CASE(VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT, "vertex shader invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT, "geometry shader invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT, "geometry shader primitives");
    CASE(VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT, "clipping invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT, "clipping primitives");
    CASE(VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT, "fragment shader invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT, "tessellation control shader patches");
    CASE(VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT, "tessellation evaluation shader invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT, "compute shader invocations");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushquerypipelinestatisticflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT, "input assembly vertices");
    CASE(VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT, "input assembly primitives");
    CASE(VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT, "vertex shader invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT, "geometry shader invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT, "geometry shader primitives");
    CASE(VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT, "clipping invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT, "clipping primitives");
    CASE(VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT, "fragment shader invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT, "tessellation control shader patches");
    CASE(VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT, "tessellation evaluation shader invocations");
    CASE(VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT, "compute shader invocations");
#undef CASE

    return n;
    }

static int QueryPipelineStatisticFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushquerypipelinestatisticflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkquerypipelinestatisticflags(L, 1));
    return 1;
    }

#define Add_QueryPipelineStatisticFlags(L)  \
    ADD(QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT);\
    ADD(QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT);\



/*----------------------------------------------------------------------*
 | VkFenceCreateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkfencecreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_FENCE_CREATE_SIGNALED_BIT, "signaled");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushfencecreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_FENCE_CREATE_SIGNALED_BIT, "signaled");
#undef CASE

    return n;
    }

static int FenceCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushfencecreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkfencecreateflags(L, 1));
    return 1;
    }

#define Add_FenceCreateFlags(L) \
    ADD(FENCE_CREATE_SIGNALED_BIT);\




/*----------------------------------------------------------------------*
 | VkSparseMemoryBindFlags
 *----------------------------------------------------------------------*/

static VkFlags checksparsememorybindflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_SPARSE_MEMORY_BIND_METADATA_BIT, "metadata");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushsparsememorybindflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_SPARSE_MEMORY_BIND_METADATA_BIT, "metadata");
#undef CASE

    return n;
    }

static int SparseMemoryBindFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsparsememorybindflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checksparsememorybindflags(L, 1));
    return 1;
    }

#define Add_SparseMemoryBindFlags(L)    \
    ADD(SPARSE_MEMORY_BIND_METADATA_BIT);\



/*----------------------------------------------------------------------*
 | VkSparseImageFormatFlags
 *----------------------------------------------------------------------*/

static VkFlags checksparseimageformatflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_SPARSE_IMAGE_FORMAT_SINGLE_MIPTAIL_BIT, "single miptail");
    CASE(VK_SPARSE_IMAGE_FORMAT_ALIGNED_MIP_SIZE_BIT, "aligned mip size");
    CASE(VK_SPARSE_IMAGE_FORMAT_NONSTANDARD_BLOCK_SIZE_BIT, "nonstandard block size");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushsparseimageformatflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_SPARSE_IMAGE_FORMAT_SINGLE_MIPTAIL_BIT, "single miptail");
    CASE(VK_SPARSE_IMAGE_FORMAT_ALIGNED_MIP_SIZE_BIT, "aligned mip size");
    CASE(VK_SPARSE_IMAGE_FORMAT_NONSTANDARD_BLOCK_SIZE_BIT, "nonstandard block size");
#undef CASE

    return n;
    }

static int SparseImageFormatFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsparseimageformatflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checksparseimageformatflags(L, 1));
    return 1;
    }

#define Add_SparseImageFormatFlags(L)   \
    ADD(SPARSE_IMAGE_FORMAT_SINGLE_MIPTAIL_BIT);\
    ADD(SPARSE_IMAGE_FORMAT_ALIGNED_MIP_SIZE_BIT);\
    ADD(SPARSE_IMAGE_FORMAT_NONSTANDARD_BLOCK_SIZE_BIT);\



/*----------------------------------------------------------------------*
 | VkImageAspectFlags
 *----------------------------------------------------------------------*/

static VkFlags checkimageaspectflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_IMAGE_ASPECT_COLOR_BIT, "color");
    CASE(VK_IMAGE_ASPECT_DEPTH_BIT, "depth");
    CASE(VK_IMAGE_ASPECT_STENCIL_BIT, "stencil");
    CASE(VK_IMAGE_ASPECT_METADATA_BIT, "metadata");
    CASE(VK_IMAGE_ASPECT_PLANE_0_BIT, "plane 0");
    CASE(VK_IMAGE_ASPECT_PLANE_1_BIT, "plane 1");
    CASE(VK_IMAGE_ASPECT_PLANE_2_BIT, "plane 2");
    CASE(VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT, "memory plane 0");
    CASE(VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT, "memory plane 1");
    CASE(VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT, "memory plane 2");
    CASE(VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT, "memory plane 3");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushimageaspectflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_IMAGE_ASPECT_COLOR_BIT, "color");
    CASE(VK_IMAGE_ASPECT_DEPTH_BIT, "depth");
    CASE(VK_IMAGE_ASPECT_STENCIL_BIT, "stencil");
    CASE(VK_IMAGE_ASPECT_METADATA_BIT, "metadata");
    CASE(VK_IMAGE_ASPECT_PLANE_0_BIT, "plane 0");
    CASE(VK_IMAGE_ASPECT_PLANE_1_BIT, "plane 1");
    CASE(VK_IMAGE_ASPECT_PLANE_2_BIT, "plane 2");
    CASE(VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT, "memory plane 0");
    CASE(VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT, "memory plane 1");
    CASE(VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT, "memory plane 2");
    CASE(VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT, "memory plane 3");
#undef CASE

    return n;
    }

static int ImageAspectFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushimageaspectflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkimageaspectflags(L, 1));
    return 1;
    }

#define Add_ImageAspectFlags(L) \
    ADD(IMAGE_ASPECT_COLOR_BIT);\
    ADD(IMAGE_ASPECT_DEPTH_BIT);\
    ADD(IMAGE_ASPECT_STENCIL_BIT);\
    ADD(IMAGE_ASPECT_METADATA_BIT);\
    ADD(IMAGE_ASPECT_PLANE_0_BIT);\
    ADD(IMAGE_ASPECT_PLANE_1_BIT);\
    ADD(IMAGE_ASPECT_PLANE_2_BIT);\
    ADD(IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT);\
    ADD(IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT);\
    ADD(IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT);\
    ADD(IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT);\

/*----------------------------------------------------------------------*
 | VkPipelineStageFlags
 *----------------------------------------------------------------------*/

static VkFlags checkpipelinestageflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, "top of pipe");
        CASE(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT, "draw indirect");
        CASE(VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, "vertex input");
        CASE(VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, "vertex shader");
        CASE(VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT, "tessellation control shader");
        CASE(VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT, "tessellation evaluation shader");
        CASE(VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT, "geometry shader");
        CASE(VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, "fragment shader");
        CASE(VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, "early fragment tests");
        CASE(VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, "late fragment tests");
        CASE(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, "color attachment output");
        CASE(VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, "compute shader");
        CASE(VK_PIPELINE_STAGE_TRANSFER_BIT, "transfer");
        CASE(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, "bottom of pipe");
        CASE(VK_PIPELINE_STAGE_HOST_BIT, "host");
        CASE(VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT, "conditional rendering");
        CASE(VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT, "transform feedback");
    // These are not individual bits:
        CASE(VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, "all graphics");
        CASE(VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, "all commands");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushpipelinestageflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, "top of pipe");
        CASE(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT, "draw indirect");
        CASE(VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, "vertex input");
        CASE(VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, "vertex shader");
        CASE(VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT, "tessellation control shader");
        CASE(VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT, "tessellation evaluation shader");
        CASE(VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT, "geometry shader");
        CASE(VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, "fragment shader");
        CASE(VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, "early fragment tests");
        CASE(VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, "late fragment tests");
        CASE(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, "color attachment output");
        CASE(VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, "compute shader");
        CASE(VK_PIPELINE_STAGE_TRANSFER_BIT, "transfer");
        CASE(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, "bottom of pipe");
        CASE(VK_PIPELINE_STAGE_HOST_BIT, "host");
        CASE(VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT, "conditional rendering");
        CASE(VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT, "transform feedback");
    // These are not individual bits:
        CASE(VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, "all graphics");
        CASE(VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, "all commands");
#undef CASE

    return n;
    }

static int PipelineStageFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushpipelinestageflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkpipelinestageflags(L, 1));
    return 1;
    }

#define Add_PipelineStageFlags(L)   \
    ADD(PIPELINE_STAGE_TOP_OF_PIPE_BIT);\
    ADD(PIPELINE_STAGE_DRAW_INDIRECT_BIT);\
    ADD(PIPELINE_STAGE_VERTEX_INPUT_BIT);\
    ADD(PIPELINE_STAGE_VERTEX_SHADER_BIT);\
    ADD(PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT);\
    ADD(PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT);\
    ADD(PIPELINE_STAGE_GEOMETRY_SHADER_BIT);\
    ADD(PIPELINE_STAGE_FRAGMENT_SHADER_BIT);\
    ADD(PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);\
    ADD(PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT);\
    ADD(PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);\
    ADD(PIPELINE_STAGE_COMPUTE_SHADER_BIT);\
    ADD(PIPELINE_STAGE_TRANSFER_BIT);\
    ADD(PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);\
    ADD(PIPELINE_STAGE_HOST_BIT);\
    ADD(PIPELINE_STAGE_ALL_GRAPHICS_BIT);\
    ADD(PIPELINE_STAGE_ALL_COMMANDS_BIT);\
    ADD(PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT);\
    ADD(PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT);\


/*----------------------------------------------------------------------*
 | VkMemoryHeapFlag
 *----------------------------------------------------------------------*/

static VkFlags checkmemoryheapflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, "device local");
        CASE(VK_MEMORY_HEAP_MULTI_INSTANCE_BIT, "multi instance");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushmemoryheapflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, "device local");
        CASE(VK_MEMORY_HEAP_MULTI_INSTANCE_BIT, "multi instance");
#undef CASE

    return n;
    }

static int MemoryHeapFlag(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushmemoryheapflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkmemoryheapflags(L, 1));
    return 1;
    }

#define Add_MemoryHeapFlag(L)   \
        ADD(MEMORY_HEAP_DEVICE_LOCAL_BIT);\
        ADD(MEMORY_HEAP_MULTI_INSTANCE_BIT);\


/*----------------------------------------------------------------------*
 | VkMemoryPropertyFlags
 *----------------------------------------------------------------------*/

static VkFlags checkmemorypropertyflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, "device local");
        CASE(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, "host visible");
        CASE(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, "host coherent");
        CASE(VK_MEMORY_PROPERTY_HOST_CACHED_BIT, "host cached");
        CASE(VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, "lazily allocated");
        CASE(VK_MEMORY_PROPERTY_PROTECTED_BIT, "protected");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushmemorypropertyflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, "device local");
        CASE(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, "host visible");
        CASE(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, "host coherent");
        CASE(VK_MEMORY_PROPERTY_HOST_CACHED_BIT, "host cached");
        CASE(VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, "lazily allocated");
        CASE(VK_MEMORY_PROPERTY_PROTECTED_BIT, "protected");
#undef CASE

    return n;
    }

static int MemoryPropertyFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushmemorypropertyflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkmemorypropertyflags(L, 1));
    return 1;
    }

#define Add_MemoryPropertyFlags(L)  \
    ADD(MEMORY_PROPERTY_DEVICE_LOCAL_BIT);\
    ADD(MEMORY_PROPERTY_HOST_VISIBLE_BIT);\
    ADD(MEMORY_PROPERTY_HOST_COHERENT_BIT);\
    ADD(MEMORY_PROPERTY_HOST_CACHED_BIT);\
    ADD(MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT);\
    ADD(MEMORY_PROPERTY_PROTECTED_BIT);\


/*----------------------------------------------------------------------*
 | VkQueueFlags
 *----------------------------------------------------------------------*/

static VkFlags checkqueueflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_QUEUE_GRAPHICS_BIT, "graphics");
        CASE(VK_QUEUE_COMPUTE_BIT, "compute");
        CASE(VK_QUEUE_TRANSFER_BIT, "transfer");
        CASE(VK_QUEUE_SPARSE_BINDING_BIT, "sparse binding");
        CASE(VK_QUEUE_PROTECTED_BIT, "protected");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushqueueflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_QUEUE_GRAPHICS_BIT, "graphics");
        CASE(VK_QUEUE_COMPUTE_BIT, "compute");
        CASE(VK_QUEUE_TRANSFER_BIT, "transfer");
        CASE(VK_QUEUE_SPARSE_BINDING_BIT, "sparse binding");
        CASE(VK_QUEUE_PROTECTED_BIT, "protected");
#undef CASE

    return n;
    }

static int QueueFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushqueueflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkqueueflags(L, 1));
    return 1;
    }

#define Add_QueueFlags(L)   \
    ADD(QUEUE_GRAPHICS_BIT);\
    ADD(QUEUE_COMPUTE_BIT);\
    ADD(QUEUE_TRANSFER_BIT);\
    ADD(QUEUE_SPARSE_BINDING_BIT);\
    ADD(QUEUE_PROTECTED_BIT);\


/*----------------------------------------------------------------------*
 | VkSampleCountFlags
 *----------------------------------------------------------------------*/

static VkFlags checksamplecountflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_SAMPLE_COUNT_1_BIT, "1");
        CASE(VK_SAMPLE_COUNT_2_BIT, "2");
        CASE(VK_SAMPLE_COUNT_4_BIT, "4");
        CASE(VK_SAMPLE_COUNT_8_BIT, "8");
        CASE(VK_SAMPLE_COUNT_16_BIT, "16");
        CASE(VK_SAMPLE_COUNT_32_BIT, "32");
        CASE(VK_SAMPLE_COUNT_64_BIT, "64");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushsamplecountflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_SAMPLE_COUNT_1_BIT, "1");
        CASE(VK_SAMPLE_COUNT_2_BIT, "2");
        CASE(VK_SAMPLE_COUNT_4_BIT, "4");
        CASE(VK_SAMPLE_COUNT_8_BIT, "8");
        CASE(VK_SAMPLE_COUNT_16_BIT, "16");
        CASE(VK_SAMPLE_COUNT_32_BIT, "32");
        CASE(VK_SAMPLE_COUNT_64_BIT, "64");
#undef CASE

    return n;
    }

static int SampleCountFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsamplecountflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checksamplecountflags(L, 1));
    return 1;
    }

#define Add_SampleCountFlags(L) \
    ADD(SAMPLE_COUNT_1_BIT);\
    ADD(SAMPLE_COUNT_2_BIT);\
    ADD(SAMPLE_COUNT_4_BIT);\
    ADD(SAMPLE_COUNT_8_BIT);\
    ADD(SAMPLE_COUNT_16_BIT);\
    ADD(SAMPLE_COUNT_32_BIT);\
    ADD(SAMPLE_COUNT_64_BIT);\



/*----------------------------------------------------------------------*
 | VkImageCreateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkimagecreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_IMAGE_CREATE_SPARSE_BINDING_BIT, "sparse binding");
        CASE(VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT, "sparse residency");
        CASE(VK_IMAGE_CREATE_SPARSE_ALIASED_BIT, "sparse aliased");
        CASE(VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT, "mutable format");
        CASE(VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, "cube compatible");
        CASE(VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT, "block texel view compatible");
        CASE(VK_IMAGE_CREATE_EXTENDED_USAGE_BIT, "extended usage");
        CASE(VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT, "sample locations compatible depth");
        CASE(VK_IMAGE_CREATE_DISJOINT_BIT, "disjoint");
        CASE(VK_IMAGE_CREATE_ALIAS_BIT, "alias");
        CASE(VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT,"split instance bind regions");
        CASE(VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT,"2d array compatible");
        CASE(VK_IMAGE_CREATE_PROTECTED_BIT, "protected");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushimagecreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_IMAGE_CREATE_SPARSE_BINDING_BIT, "sparse binding");
        CASE(VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT, "sparse residency");
        CASE(VK_IMAGE_CREATE_SPARSE_ALIASED_BIT, "sparse aliased");
        CASE(VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT, "mutable format");
        CASE(VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, "cube compatible");
        CASE(VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT, "block texel view compatible");
        CASE(VK_IMAGE_CREATE_EXTENDED_USAGE_BIT, "extended usage");
        CASE(VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT, "sample locations compatible depth");
        CASE(VK_IMAGE_CREATE_DISJOINT_BIT, "disjoint");
        CASE(VK_IMAGE_CREATE_ALIAS_BIT, "alias");
        CASE(VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT,"split instance bind regions");
        CASE(VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT,"2d array compatible");
        CASE(VK_IMAGE_CREATE_PROTECTED_BIT, "protected");
#undef CASE

    return n;
    }

static int ImageCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushimagecreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkimagecreateflags(L, 1));
    return 1;
    }

#define Add_ImageCreateFlags(L) \
    ADD(IMAGE_CREATE_SPARSE_BINDING_BIT);\
    ADD(IMAGE_CREATE_SPARSE_RESIDENCY_BIT);\
    ADD(IMAGE_CREATE_SPARSE_ALIASED_BIT);\
    ADD(IMAGE_CREATE_MUTABLE_FORMAT_BIT);\
    ADD(IMAGE_CREATE_CUBE_COMPATIBLE_BIT);\
    ADD(IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT);\
    ADD(IMAGE_CREATE_EXTENDED_USAGE_BIT);\
    ADD(IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT);\
    ADD(IMAGE_CREATE_DISJOINT_BIT);\
    ADD(IMAGE_CREATE_ALIAS_BIT);\
    ADD(IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT);\
    ADD(IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT);\
    ADD(IMAGE_CREATE_PROTECTED_BIT);\

/*----------------------------------------------------------------------*
 | VkImageUsageFlags
 *----------------------------------------------------------------------*/

static VkFlags checkimageusageflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_IMAGE_USAGE_TRANSFER_SRC_BIT, "transfer src");
        CASE(VK_IMAGE_USAGE_TRANSFER_DST_BIT, "transfer dst");
        CASE(VK_IMAGE_USAGE_SAMPLED_BIT, "sampled");
        CASE(VK_IMAGE_USAGE_STORAGE_BIT, "storage");
        CASE(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, "color attachment");
        CASE(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, "depth stencil attachment");
        CASE(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, "transient attachment");
        CASE(VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, "input attachment");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushimageusageflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_IMAGE_USAGE_TRANSFER_SRC_BIT, "transfer src");
        CASE(VK_IMAGE_USAGE_TRANSFER_DST_BIT, "transfer dst");
        CASE(VK_IMAGE_USAGE_SAMPLED_BIT, "sampled");
        CASE(VK_IMAGE_USAGE_STORAGE_BIT, "storage");
        CASE(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, "color attachment");
        CASE(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, "depth stencil attachment");
        CASE(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, "transient attachment");
        CASE(VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, "input attachment");
#undef CASE

    return n;
    }

static int ImageUsageFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushimageusageflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkimageusageflags(L, 1));
    return 1;
    }

#define Add_ImageUsageFlags(L)  \
    ADD(IMAGE_USAGE_TRANSFER_SRC_BIT);\
    ADD(IMAGE_USAGE_TRANSFER_DST_BIT);\
    ADD(IMAGE_USAGE_SAMPLED_BIT);\
    ADD(IMAGE_USAGE_STORAGE_BIT);\
    ADD(IMAGE_USAGE_COLOR_ATTACHMENT_BIT);\
    ADD(IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);\
    ADD(IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT);\
    ADD(IMAGE_USAGE_INPUT_ATTACHMENT_BIT);\



/*----------------------------------------------------------------------*
 | VkFormatFeatureFlags                                                 |
 *----------------------------------------------------------------------*/


static VkFlags checkformatfeatureflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT, "sampled image");
        CASE(VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT, "storage image");
        CASE(VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT, "storage image atomic");
        CASE(VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT, "uniform texel buffer");
        CASE(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT, "storage texel buffer");
        CASE(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT, "storage texel buffer atomic");
        CASE(VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT, "vertex buffer");
        CASE(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT, "color attachment");
        CASE(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT, "color attachment blend");
        CASE(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, "depth stencil attachment");
        CASE(VK_FORMAT_FEATURE_BLIT_SRC_BIT, "blit src");
        CASE(VK_FORMAT_FEATURE_BLIT_DST_BIT, "blit dst");
        CASE(VK_FORMAT_FEATURE_TRANSFER_SRC_BIT, "transfer src");
        CASE(VK_FORMAT_FEATURE_TRANSFER_DST_BIT, "transfer dst");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT, "sampled image filter linear");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT , "sampled image filter minmax");
        CASE(VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT,"midpoint chroma samples");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT,"sampled image ycbcr conversion linear filter");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT,"sampled image ycbcr conversion separate reconstruction filter");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT,"sampled image ycbcr conversion chroma reconstruction explicit");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT,"sampled image ycbcr conversion chroma reconstruction explicit forceable");
        CASE(VK_FORMAT_FEATURE_DISJOINT_BIT,"disjoint");
        CASE(VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT,"cosited chroma samples");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }


static int pushformatfeatureflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT, "sampled image");
        CASE(VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT, "storage image");
        CASE(VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT, "storage image atomic");
        CASE(VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT, "uniform texel buffer");
        CASE(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT, "storage texel buffer");
        CASE(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT, "storage texel buffer atomic");
        CASE(VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT, "vertex buffer");
        CASE(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT, "color attachment");
        CASE(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT, "color attachment blend");
        CASE(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, "depth stencil attachment");
        CASE(VK_FORMAT_FEATURE_BLIT_SRC_BIT, "blit src");
        CASE(VK_FORMAT_FEATURE_BLIT_DST_BIT, "blit dst");
        CASE(VK_FORMAT_FEATURE_TRANSFER_SRC_BIT, "transfer src");
        CASE(VK_FORMAT_FEATURE_TRANSFER_DST_BIT, "transfer dst");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT, "sampled image filter linear");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT , "sampled image filter minmax");
        CASE(VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT,"midpoint chroma samples");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT,"sampled image ycbcr conversion linear filter");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT,"sampled image ycbcr conversion separate reconstruction filter");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT,"sampled image ycbcr conversion chroma reconstruction explicit");
        CASE(VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT,"sampled image ycbcr conversion chroma reconstruction explicit forceable");
        CASE(VK_FORMAT_FEATURE_DISJOINT_BIT,"disjoint");
        CASE(VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT,"cosited chroma samples");
#undef CASE

    return n;
    }

static int FormatFeatureFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushformatfeatureflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkformatfeatureflags(L, 1));
    return 1;
    }

#define Add_FormatFeatureFlags(L)   \
    ADD(FORMAT_FEATURE_SAMPLED_IMAGE_BIT);\
    ADD(FORMAT_FEATURE_STORAGE_IMAGE_BIT);\
    ADD(FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT);\
    ADD(FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT);\
    ADD(FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT);\
    ADD(FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT);\
    ADD(FORMAT_FEATURE_VERTEX_BUFFER_BIT);\
    ADD(FORMAT_FEATURE_COLOR_ATTACHMENT_BIT);\
    ADD(FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT);\
    ADD(FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);\
    ADD(FORMAT_FEATURE_BLIT_SRC_BIT);\
    ADD(FORMAT_FEATURE_BLIT_DST_BIT);\
    ADD(FORMAT_FEATURE_TRANSFER_SRC_BIT);\
    ADD(FORMAT_FEATURE_TRANSFER_DST_BIT);\
    ADD(FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);\
    ADD(FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT);\
    ADD(FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT);\
    ADD(FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT);\
    ADD(FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT);\
    ADD(FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT);\
    ADD(FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT);\
    ADD(FORMAT_FEATURE_DISJOINT_BIT);\
    ADD(FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT);\

/*----------------------------------------------------------------------*
 | VkSurfaceTransformFlagsKHR
 *----------------------------------------------------------------------*/

static VkFlags checksurfacetransformflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, "identity");
    CASE(VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR, "rotate 90");
    CASE(VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR, "rotate 180");
    CASE(VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR, "rotate 270");
    CASE(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR, "horizontal mirror");
    CASE(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR, "horizontal mirror rotate 90");
    CASE(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR, "horizontal mirror rotate 180");
    CASE(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR, "horizontal mirror rotate 270");
    CASE(VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR, "inherit");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushsurfacetransformflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, "identity");
    CASE(VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR, "rotate 90");
    CASE(VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR, "rotate 180");
    CASE(VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR, "rotate 270");
    CASE(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR, "horizontal mirror");
    CASE(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR, "horizontal mirror rotate 90");
    CASE(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR, "horizontal mirror rotate 180");
    CASE(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR, "horizontal mirror rotate 270");
    CASE(VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR, "inherit");
#undef CASE

    return n;
    }

static int SurfaceTransformFlagsKHR(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsurfacetransformflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checksurfacetransformflags(L, 1));
    return 1;
    }

#define Add_SurfaceTransformFlagsKHR(L) \
    ADD(SURFACE_TRANSFORM_IDENTITY_BIT_KHR); \
    ADD(SURFACE_TRANSFORM_ROTATE_90_BIT_KHR); \
    ADD(SURFACE_TRANSFORM_ROTATE_180_BIT_KHR); \
    ADD(SURFACE_TRANSFORM_ROTATE_270_BIT_KHR); \
    ADD(SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR);\
    ADD(SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR);\
    ADD(SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR);\
    ADD(SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR);\
    ADD(SURFACE_TRANSFORM_INHERIT_BIT_KHR);\


/*----------------------------------------------------------------------*
 | VkCompositeAlphaFlagsKHR
 *----------------------------------------------------------------------*/

static VkFlags checkcompositealphaflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, "opaque");
    CASE(VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR, "pre multiplied");
    CASE(VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR, "post multiplied");
    CASE(VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR, "inherit");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushcompositealphaflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, "opaque");
    CASE(VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR, "pre multiplied");
    CASE(VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR, "post multiplied");
    CASE(VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR, "inherit");
#undef CASE

    return n;
    }

static int CompositeAlphaFlagsKHR(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushcompositealphaflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkcompositealphaflags(L, 1));
    return 1;
    }

#define Add_CompositeAlphaFlagKHR(L)    \
    ADD(COMPOSITE_ALPHA_OPAQUE_BIT_KHR);\
    ADD(COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR);\
    ADD(COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR);\
    ADD(COMPOSITE_ALPHA_INHERIT_BIT_KHR);\

/*----------------------------------------------------------------------*
 | VkDisplayPlaneAlphaFlagsKHR
 *----------------------------------------------------------------------*/

static VkFlags checkdisplayplanealphaflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR, "opaque");
    CASE(VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR, "global");
    CASE(VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR, "per pixel");
    CASE(VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR, "per pixel premultiplied");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdisplayplanealphaflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR, "opaque");
    CASE(VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR, "global");
    CASE(VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR, "per pixel");
    CASE(VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR, "per pixel premultiplied");
#undef CASE

    return n;
    }

static int DisplayPlaneAlphaFlagsKHR(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdisplayplanealphaflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdisplayplanealphaflags(L, 1));
    return 1;
    }

#define Add_DisplayPlaneAlphaFlagsKHR(L)    \
    ADD(DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR);\
    ADD(DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR);\
    ADD(DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR);\
    ADD(DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR);\



/*----------------------------------------------------------------------*
 | VkDebugReportFlagsEXT
 *----------------------------------------------------------------------*/

static VkFlags checkdebugreportflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_DEBUG_REPORT_INFORMATION_BIT_EXT, "information");
    CASE(VK_DEBUG_REPORT_WARNING_BIT_EXT, "warning");
    CASE(VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT, "performance warning");
    CASE(VK_DEBUG_REPORT_ERROR_BIT_EXT, "error");
    CASE(VK_DEBUG_REPORT_DEBUG_BIT_EXT, "debug");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdebugreportflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_DEBUG_REPORT_INFORMATION_BIT_EXT, "information");
    CASE(VK_DEBUG_REPORT_WARNING_BIT_EXT, "warning");
    CASE(VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT, "performance warning");
    CASE(VK_DEBUG_REPORT_ERROR_BIT_EXT, "error");
    CASE(VK_DEBUG_REPORT_DEBUG_BIT_EXT, "debug");
#undef CASE

    return n;
    }

static int DebugReportFlagsEXT(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdebugreportflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdebugreportflags(L, 1));
    return 1;
    }

#define Add_DebugReportFlagsEXT(L)  \
    ADD(DEBUG_REPORT_INFORMATION_BIT_EXT);\
    ADD(DEBUG_REPORT_WARNING_BIT_EXT);\
    ADD(DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT);\
    ADD(DEBUG_REPORT_ERROR_BIT_EXT);\
    ADD(DEBUG_REPORT_DEBUG_BIT_EXT);\

/*----------------------------------------------------------------------*
 | VkSurfaceCounterFlagsEXT
 *----------------------------------------------------------------------*/

static VkFlags checksurfacecounterflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_SURFACE_COUNTER_VBLANK_EXT, "vblank");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushsurfacecounterflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_SURFACE_COUNTER_VBLANK_EXT, "vblank");
#undef CASE

    return n;
    }

static int SurfaceCounterFlagsEXT(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsurfacecounterflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checksurfacecounterflags(L, 1));
    return 1;
    }

#define Add_SurfaceCounterFlagsEXT(L)   \
    ADD(SURFACE_COUNTER_VBLANK_EXT);\


/*----------------------------------------------------------------------*
 | VkExternalMemoryHandleTypeFlags
 *----------------------------------------------------------------------*/

static VkFlags checkexternalmemoryhandletypeflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT, "opaque fd");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT, "opaque win32");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT, "opaque win32 kmt");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT, "d3d11 texture");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT, "d3d11 texture kmt");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_HEAP_BIT, "d3d12 heap");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_RESOURCE_BIT, "d3d12 resource");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushexternalmemoryhandletypeflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT, "opaque fd");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT, "opaque win32");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT, "opaque win32 kmt");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT, "d3d11 texture");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT, "d3d11 texture kmt");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_HEAP_BIT, "d3d12 heap");
    CASE(VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_RESOURCE_BIT, "d3d12 resource");
#undef CASE

    return n;
    }

static int ExternalMemoryHandleTypeFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushexternalmemoryhandletypeflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkexternalmemoryhandletypeflags(L, 1));
    return 1;
    }

#define Add_ExternalMemoryHandleTypeFlags(L) \
    ADD(EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT); \
    ADD(EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT); \
    ADD(EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT); \
    ADD(EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT); \
    ADD(EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT); \
    ADD(EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_HEAP_BIT); \
    ADD(EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_RESOURCE_BIT); \

/*----------------------------------------------------------------------*
 | VkExternalMemoryFeatureFlags
 *----------------------------------------------------------------------*/

static VkFlags checkexternalmemoryfeatureflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT, "dedicated only");
    CASE(VK_EXTERNAL_MEMORY_FEATURE_EXPORTABLE_BIT, "exportable");
    CASE(VK_EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT, "importable");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushexternalmemoryfeatureflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT, "dedicated only");
    CASE(VK_EXTERNAL_MEMORY_FEATURE_EXPORTABLE_BIT, "exportable");
    CASE(VK_EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT, "importable");
#undef CASE

    return n;
    }

static int ExternalMemoryFeatureFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushexternalmemoryfeatureflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkexternalmemoryfeatureflags(L, 1));
    return 1;
    }

#define Add_ExternalMemoryFeatureFlags(L) \
    ADD(EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT); \
    ADD(EXTERNAL_MEMORY_FEATURE_EXPORTABLE_BIT); \
    ADD(EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT); \


/*----------------------------------------------------------------------*
 | VkExternalSemaphoreHandleTypeFlags
 *----------------------------------------------------------------------*/

static VkFlags checkexternalsemaphorehandletypeflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT, "opaque fd");
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT, "opaque win32");
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT, "opaque win32 kmt");
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT, "d3d12 fence");
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT, "sync fd");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushexternalsemaphorehandletypeflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT, "opaque fd");
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT, "opaque win32");
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT, "opaque win32 kmt");
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT, "d3d12 fence");
    CASE(VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT, "sync fd");
#undef CASE

    return n;
    }

static int ExternalSemaphoreHandleTypeFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushexternalsemaphorehandletypeflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkexternalsemaphorehandletypeflags(L, 1));
    return 1;
    }

#define Add_ExternalSemaphoreHandleTypeFlags(L) \
    ADD(EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT); \
    ADD(EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT); \
    ADD(EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT); \
    ADD(EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT); \
    ADD(EXTERNAL_SEMAPHORE_HANDLE_TYPE_SYNC_FD_BIT); \



/*----------------------------------------------------------------------*
 | VkExternalSemaphoreFeatureFlags
 *----------------------------------------------------------------------*/

static VkFlags checkexternalsemaphorefeatureflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_EXTERNAL_SEMAPHORE_FEATURE_EXPORTABLE_BIT, "exportable");
    CASE(VK_EXTERNAL_SEMAPHORE_FEATURE_IMPORTABLE_BIT, "importable");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushexternalsemaphorefeatureflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_EXTERNAL_SEMAPHORE_FEATURE_EXPORTABLE_BIT, "exportable");
    CASE(VK_EXTERNAL_SEMAPHORE_FEATURE_IMPORTABLE_BIT, "importable");
#undef CASE

    return n;
    }

static int ExternalSemaphoreFeatureFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushexternalsemaphorefeatureflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkexternalsemaphorefeatureflags(L, 1));
    return 1;
    }

#define Add_ExternalSemaphoreFeatureFlags(L) \
    ADD(EXTERNAL_SEMAPHORE_FEATURE_EXPORTABLE_BIT); \
    ADD(EXTERNAL_SEMAPHORE_FEATURE_IMPORTABLE_BIT); \


/*----------------------------------------------------------------------*
 | VkSemaphoreImportFlags
 *----------------------------------------------------------------------*/

static VkFlags checksemaphoreimportflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_SEMAPHORE_IMPORT_TEMPORARY_BIT, "temporary");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushsemaphoreimportflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_SEMAPHORE_IMPORT_TEMPORARY_BIT, "temporary");
#undef CASE

    return n;
    }

static int SemaphoreImportFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsemaphoreimportflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checksemaphoreimportflags(L, 1));
    return 1;
    }

#define Add_SemaphoreImportFlags(L) \
    ADD(SEMAPHORE_IMPORT_TEMPORARY_BIT); \


/*----------------------------------------------------------------------*
 | VkExternalFenceHandleTypeFlags
 *----------------------------------------------------------------------*/

static VkFlags checkexternalfencehandletypeflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT, "opaque_fd");
    CASE(VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT, "opaque_win32");
    CASE(VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT, "opaque_win32_kmt");
    CASE(VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT, "sync_fd");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushexternalfencehandletypeflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT, "opaque_fd");
    CASE(VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT, "opaque_win32");
    CASE(VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT, "opaque_win32_kmt");
    CASE(VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT, "sync_fd");
#undef CASE

    return n;
    }

static int ExternalFenceHandleTypeFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushexternalfencehandletypeflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkexternalfencehandletypeflags(L, 1));
    return 1;
    }

#define Add_ExternalFenceHandleTypeFlags(L) \
    ADD(EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT); \
    ADD(EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT); \
    ADD(EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_KMT_BIT); \
    ADD(EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT); \

/*----------------------------------------------------------------------*
 | VkExternalFenceFeatureFlags
 *----------------------------------------------------------------------*/

static VkFlags checkexternalfencefeatureflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_EXTERNAL_FENCE_FEATURE_EXPORTABLE_BIT, "exportable");
    CASE(VK_EXTERNAL_FENCE_FEATURE_IMPORTABLE_BIT, "importable");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushexternalfencefeatureflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_EXTERNAL_FENCE_FEATURE_EXPORTABLE_BIT, "exportable");
    CASE(VK_EXTERNAL_FENCE_FEATURE_IMPORTABLE_BIT, "importable");
#undef CASE

    return n;
    }

static int ExternalFenceFeatureFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushexternalfencefeatureflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkexternalfencefeatureflags(L, 1));
    return 1;
    }

#define Add_ExternalFenceFeatureFlags(L) \
    ADD(EXTERNAL_FENCE_FEATURE_EXPORTABLE_BIT); \
    ADD(EXTERNAL_FENCE_FEATURE_IMPORTABLE_BIT); \

/*----------------------------------------------------------------------*
 | VkFenceImportFlags
 *----------------------------------------------------------------------*/

static VkFlags checkfenceimportflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_FENCE_IMPORT_TEMPORARY_BIT, "temporary");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushfenceimportflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_FENCE_IMPORT_TEMPORARY_BIT, "temporary");
#undef CASE

    return n;
    }

static int FenceImportFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushfenceimportflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkfenceimportflags(L, 1));
    return 1;
    }

#define Add_FenceImportFlags(L) \
    ADD(FENCE_IMPORT_TEMPORARY_BIT); \


/*----------------------------------------------------------------------*
 | VkDeviceQueueCreateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkdevicequeuecreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT, "protected");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdevicequeuecreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT, "protected");
#undef CASE

    return n;
    }

static int DeviceQueueCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdevicequeuecreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdevicequeuecreateflags(L, 1));
    return 1;
    }

#define Add_DeviceQueueCreateFlags(L) \
    ADD(DEVICE_QUEUE_CREATE_PROTECTED_BIT);\

/*----------------------------------------------------------------------*
 | VkSubgroupFeatureFlags
 *----------------------------------------------------------------------*/

static VkFlags checksubgroupfeatureflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_SUBGROUP_FEATURE_BASIC_BIT, "basic");
        CASE(VK_SUBGROUP_FEATURE_VOTE_BIT, "vote");
        CASE(VK_SUBGROUP_FEATURE_ARITHMETIC_BIT, "arithmetic");
        CASE(VK_SUBGROUP_FEATURE_BALLOT_BIT, "ballot");
        CASE(VK_SUBGROUP_FEATURE_SHUFFLE_BIT, "shuffle");
        CASE(VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT, "shuffle relative");
        CASE(VK_SUBGROUP_FEATURE_CLUSTERED_BIT, "clustered");
        CASE(VK_SUBGROUP_FEATURE_QUAD_BIT, "quad");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushsubgroupfeatureflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_SUBGROUP_FEATURE_BASIC_BIT, "basic");
        CASE(VK_SUBGROUP_FEATURE_VOTE_BIT, "vote");
        CASE(VK_SUBGROUP_FEATURE_ARITHMETIC_BIT, "arithmetic");
        CASE(VK_SUBGROUP_FEATURE_BALLOT_BIT, "ballot");
        CASE(VK_SUBGROUP_FEATURE_SHUFFLE_BIT, "shuffle");
        CASE(VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT, "shuffle relative");
        CASE(VK_SUBGROUP_FEATURE_CLUSTERED_BIT, "clustered");
        CASE(VK_SUBGROUP_FEATURE_QUAD_BIT, "quad");
#undef CASE

    return n;
    }

static int SubgroupFeatureFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsubgroupfeatureflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checksubgroupfeatureflags(L, 1));
    return 1;
    }

#define Add_SubgroupFeatureFlags(L) \
    ADD(SUBGROUP_FEATURE_BASIC_BIT);\
    ADD(SUBGROUP_FEATURE_VOTE_BIT);\
    ADD(SUBGROUP_FEATURE_ARITHMETIC_BIT);\
    ADD(SUBGROUP_FEATURE_BALLOT_BIT);\
    ADD(SUBGROUP_FEATURE_SHUFFLE_BIT);\
    ADD(SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT);\
    ADD(SUBGROUP_FEATURE_CLUSTERED_BIT);\
    ADD(SUBGROUP_FEATURE_QUAD_BIT);\

/*----------------------------------------------------------------------*
 | VkPeerMemoryFeatureFlags
 *----------------------------------------------------------------------*/

static VkFlags checkpeermemoryfeatureflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_PEER_MEMORY_FEATURE_COPY_SRC_BIT, "copy src");
        CASE(VK_PEER_MEMORY_FEATURE_COPY_DST_BIT, "copy dst");
        CASE(VK_PEER_MEMORY_FEATURE_GENERIC_SRC_BIT, "generic src");
        CASE(VK_PEER_MEMORY_FEATURE_GENERIC_DST_BIT, "generic dst");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushpeermemoryfeatureflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_PEER_MEMORY_FEATURE_COPY_SRC_BIT, "copy src");
        CASE(VK_PEER_MEMORY_FEATURE_COPY_DST_BIT, "copy dst");
        CASE(VK_PEER_MEMORY_FEATURE_GENERIC_SRC_BIT, "generic src");
        CASE(VK_PEER_MEMORY_FEATURE_GENERIC_DST_BIT, "generic dst");
#undef CASE

    return n;
    }

static int PeerMemoryFeatureFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushpeermemoryfeatureflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkpeermemoryfeatureflags(L, 1));
    return 1;
    }

#define Add_PeerMemoryFeatureFlags(L) \
    ADD(PEER_MEMORY_FEATURE_COPY_SRC_BIT);\
    ADD(PEER_MEMORY_FEATURE_COPY_DST_BIT);\
    ADD(PEER_MEMORY_FEATURE_GENERIC_SRC_BIT);\
    ADD(PEER_MEMORY_FEATURE_GENERIC_DST_BIT);\

/*----------------------------------------------------------------------*
 | VkMemoryAllocateFlags
 *----------------------------------------------------------------------*/

static VkFlags checkmemoryallocateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT, "device mask");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushmemoryallocateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT, "device mask");
#undef CASE

    return n;
    }

static int MemoryAllocateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushmemoryallocateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkmemoryallocateflags(L, 1));
    return 1;
    }

#define Add_MemoryAllocateFlags(L) \
    ADD(MEMORY_ALLOCATE_DEVICE_MASK_BIT);\

/*----------------------------------------------------------------------*
 | VkDebugUtilsMessageSeverityFlagsEXT
 *----------------------------------------------------------------------*/

static VkFlags checkdebugutilsmessageseverityflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT, "verbose");
        CASE(VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, "info");
        CASE(VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT, "warning");
        CASE(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, "error");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdebugutilsmessageseverityflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT, "verbose");
        CASE(VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, "info");
        CASE(VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT, "warning");
        CASE(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, "error");
#undef CASE

    return n;
    }

static int DebugUtilsMessageSeverityFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdebugutilsmessageseverityflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdebugutilsmessageseverityflags(L, 1));
    return 1;
    }

#define Add_DebugUtilsMessageSeverityFlags(L) \
    ADD(DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT);\
    ADD(DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT);\
    ADD(DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT);\
    ADD(DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);\

/*----------------------------------------------------------------------*
 | VkDebugUtilsMessageTypeFlagsEXT
 *----------------------------------------------------------------------*/

static VkFlags checkdebugutilsmessagetypeflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT, "general");
        CASE(VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT, "validation");
        CASE(VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT, "performance");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdebugutilsmessagetypeflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT, "general");
        CASE(VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT, "validation");
        CASE(VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT, "performance");
#undef CASE

    return n;
    }

static int DebugUtilsMessageTypeFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdebugutilsmessagetypeflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdebugutilsmessagetypeflags(L, 1));
    return 1;
    }

#define Add_DebugUtilsMessageTypeFlags(L) \
    ADD(DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT);\
    ADD(DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT);\
    ADD(DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);\

/*----------------------------------------------------------------------*
 | VkDeviceGroupPresentModeFlagsKHR
 *----------------------------------------------------------------------*/

static VkFlags checkdevicegrouppresentmodeflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_DEVICE_GROUP_PRESENT_MODE_LOCAL_BIT_KHR, "local");
        CASE(VK_DEVICE_GROUP_PRESENT_MODE_REMOTE_BIT_KHR, "remote");
        CASE(VK_DEVICE_GROUP_PRESENT_MODE_SUM_BIT_KHR, "sum");
        CASE(VK_DEVICE_GROUP_PRESENT_MODE_LOCAL_MULTI_DEVICE_BIT_KHR, "local multi device");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdevicegrouppresentmodeflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_DEVICE_GROUP_PRESENT_MODE_LOCAL_BIT_KHR, "local");
        CASE(VK_DEVICE_GROUP_PRESENT_MODE_REMOTE_BIT_KHR, "remote");
        CASE(VK_DEVICE_GROUP_PRESENT_MODE_SUM_BIT_KHR, "sum");
        CASE(VK_DEVICE_GROUP_PRESENT_MODE_LOCAL_MULTI_DEVICE_BIT_KHR, "local multi device");
#undef CASE

    return n;
    }

static int DeviceGroupPresentModeFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdevicegrouppresentmodeflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdevicegrouppresentmodeflags(L, 1));
    return 1;
    }

#define Add_DeviceGroupPresentModeFlags(L) \
    ADD(DEVICE_GROUP_PRESENT_MODE_LOCAL_BIT_KHR);\
    ADD(DEVICE_GROUP_PRESENT_MODE_REMOTE_BIT_KHR);\
    ADD(DEVICE_GROUP_PRESENT_MODE_SUM_BIT_KHR);\
    ADD(DEVICE_GROUP_PRESENT_MODE_LOCAL_MULTI_DEVICE_BIT_KHR);\


/*----------------------------------------------------------------------*
 | VkSwapchainCreateFlagsKHR
 *----------------------------------------------------------------------*/

static VkFlags checkswapchaincreateflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_SWAPCHAIN_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR, "split instance bind regions");
        CASE(VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR, "protected");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushswapchaincreateflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_SWAPCHAIN_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR, "split instance bind regions");
        CASE(VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR, "protected");
#undef CASE

    return n;
    }

static int SwapchainCreateFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushswapchaincreateflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkswapchaincreateflags(L, 1));
    return 1;
    }

#define Add_SwapchainCreateFlags(L) \
    ADD(SWAPCHAIN_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR);\
    ADD(SWAPCHAIN_CREATE_PROTECTED_BIT_KHR);\

/*----------------------------------------------------------------------*
 | VkDescriptorBindingFlagsEXT
 *----------------------------------------------------------------------*/

static VkFlags checkdescriptorbindingflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT, "update after bind");
        CASE(VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT_EXT, "update unused while pending");
        CASE(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT, "partially bound");
        CASE(VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT, "variable descriptor count");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushdescriptorbindingflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT, "update after bind");
        CASE(VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT_EXT, "update unused while pending");
        CASE(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT, "partially bound");
        CASE(VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT, "variable descriptor count");
#undef CASE

    return n;
    }

static int DescriptorBindingFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushdescriptorbindingflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkdescriptorbindingflags(L, 1));
    return 1;
    }

#define Add_DescriptorBindingFlags(L) \
    ADD(DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT);\
    ADD(DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT_EXT);\
    ADD(DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT);\
    ADD(DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT);\

/*----------------------------------------------------------------------*
 | VkConditionalRenderingFlagsEXT
 *----------------------------------------------------------------------*/

static VkFlags checkconditionalrenderingflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_CONDITIONAL_RENDERING_INVERTED_BIT_EXT, "inverted");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushconditionalrenderingflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(VK_CONDITIONAL_RENDERING_INVERTED_BIT_EXT, "inverted");
#undef CASE

    return n;
    }

static int ConditionalRenderingFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushconditionalrenderingflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkconditionalrenderingflags(L, 1));
    return 1;
    }

#define Add_ConditionalRenderingFlags(L) \
    ADD(CONDITIONAL_RENDERING_INVERTED_BIT_EXT);\


/*------------------------------------------------------------------------------*
 | Additional utilities                                                         |
 *------------------------------------------------------------------------------*/

static int AddConstants(lua_State *L) /* vk.XXX constants for VK_XXX values */
    {
    Add_StencilFaceFlags(L);
    Add_CommandBufferResetFlags(L);
    Add_QueryControlFlags(L);
    Add_CommandBufferUsageFlags(L);
    Add_CommandPoolResetFlags(L);
    Add_CommandPoolCreateFlags(L);
    Add_DependencyFlags(L);
    Add_AccessFlags(L);
    Add_AttachmentDescriptionFlags(L);
    Add_DescriptorPoolCreateFlags(L);
    Add_DescriptorSetLayoutCreateFlags(L);
    Add_ColorComponentFlags(L);
    Add_CullModeFlags(L);
    Add_ShaderStageFlags(L);
    Add_PipelineCreateFlags(L);
    Add_BufferUsageFlags(L);
    Add_BufferCreateFlags(L);
    Add_QueryResultFlags(L);
    Add_QueryPipelineStatisticFlags(L);
    Add_FenceCreateFlags(L);
    Add_SparseMemoryBindFlags(L);
    Add_SparseImageFormatFlags(L);
    Add_ImageAspectFlags(L);
    Add_PipelineStageFlags(L);
    Add_MemoryHeapFlag(L);
    Add_MemoryPropertyFlags(L);
    Add_QueueFlags(L);
    Add_SampleCountFlags(L);
    Add_ImageCreateFlags(L);
    Add_ImageUsageFlags(L);
    Add_FormatFeatureFlags(L);
    Add_DeviceQueueCreateFlags(L);
    Add_SubgroupFeatureFlags(L);
    Add_PeerMemoryFeatureFlags(L);
    Add_MemoryAllocateFlags(L);
    /* extensions */
    Add_SurfaceTransformFlagsKHR(L);
    Add_CompositeAlphaFlagKHR(L);
    Add_DisplayPlaneAlphaFlagsKHR(L);
    Add_DebugReportFlagsEXT(L);
    Add_SurfaceCounterFlagsEXT(L);
    Add_ExternalMemoryHandleTypeFlags(L);
    Add_ExternalMemoryFeatureFlags(L);
    Add_ExternalSemaphoreHandleTypeFlags(L);
    Add_ExternalSemaphoreFeatureFlags(L);
    Add_SemaphoreImportFlags(L);
    Add_ExternalFenceHandleTypeFlags(L);
    Add_ExternalFenceFeatureFlags(L);
    Add_FenceImportFlags(L);
    Add_DebugUtilsMessageSeverityFlags(L);
    Add_DebugUtilsMessageTypeFlags(L);
    Add_DeviceGroupPresentModeFlags(L);
    Add_SwapchainCreateFlags(L);
    Add_DescriptorBindingFlags(L);
    Add_ConditionalRenderingFlags(L);
    return 0;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "stencilfaceflags", StencilFaceFlags },
        { "commandbufferresetflags", CommandBufferResetFlags },
        { "querycontrolflags", QueryControlFlags },
        { "commandbufferusageflags", CommandBufferUsageFlags },
        { "commandpoolresetflags", CommandPoolResetFlags },
        { "commandpoolcreateflags", CommandPoolCreateFlags },
        { "dependencyflags", DependencyFlags },
        { "accessflags", AccessFlags },
        { "attachmentdescriptionflags", AttachmentDescriptionFlags },
        { "descriptorpoolcreateflags", DescriptorPoolCreateFlags },
        { "descriptorsetlayoutcreateflags", DescriptorSetLayoutCreateFlags },
        { "colorcomponentflags", ColorComponentFlags },
        { "cullmodeflags", CullModeFlags },
        { "shaderstageflags", ShaderStageFlags },
        { "pipelinecreateflags", PipelineCreateFlags },
        { "bufferusageflags", BufferUsageFlags },
        { "buffercreateflags", BufferCreateFlags },
        { "queryresultflags", QueryResultFlags },
        { "querypipelinestatisticflags", QueryPipelineStatisticFlags },
        { "fencecreateflags", FenceCreateFlags },
        { "sparsememorybindflags", SparseMemoryBindFlags },
        { "sparseimageformatflags", SparseImageFormatFlags },
        { "imageaspectflags", ImageAspectFlags },
        { "pipelinestageflags", PipelineStageFlags },
        { "memoryheapflags", MemoryHeapFlag},
        { "memorypropertyflags", MemoryPropertyFlags },
        { "queueflags", QueueFlags },
        { "samplecountflags", SampleCountFlags },
        { "imagecreateflags", ImageCreateFlags },
        { "imageusageflags", ImageUsageFlags },
        { "formatfeatureflags", FormatFeatureFlags },
        { "devicequeuecreateflags", DeviceQueueCreateFlags },
        { "subgroupfeatureflags", SubgroupFeatureFlags },
        { "peermemoryfeatureflags", PeerMemoryFeatureFlags },
        { "memoryallocateflags", MemoryAllocateFlags },
        /* Reserved flags */
        { "instancecreateflags", ReservedFlags },
        { "devicecreateflags", ReservedFlags },
        { "memorymapflags", ReservedFlags },
        { "semaphorecreateflags", ReservedFlags },
        { "eventcreateflags", ReservedFlags },
        { "querypoolcreateflags", ReservedFlags },
        { "bufferviewcreateflags", ReservedFlags },
        { "imageviewcreateflags", ReservedFlags },
        { "shadermodulecreateflags", ReservedFlags },
        { "pipelinecachecreateflags", ReservedFlags },
        { "pipelinevertexinputstatecreateflags", ReservedFlags },
        { "pipelineshaderstagecreateflags", ReservedFlags },
        { "pipelineinputassemblystatecreateflags", ReservedFlags },
        { "pipelinetessellationstatecreateflags", ReservedFlags },
        { "pipelineviewportstatecreateflags", ReservedFlags },
        { "pipelinerasterizationstatecreateflags", ReservedFlags },
        { "pipelinemultisamplestatecreateflags", ReservedFlags },
        { "pipelinedepthstencilstatecreateflags", ReservedFlags },
        { "pipelinecolorblendstatecreateflags", ReservedFlags },
        { "pipelinedynamicstatecreateflags", ReservedFlags },
        { "pipelinelayoutcreateflags", ReservedFlags },
        { "samplercreateflags", ReservedFlags },
        { "descriptorpoolresetflags", ReservedFlags },
        { "framebuffercreateflags", ReservedFlags },
        { "renderpasscreateflags", ReservedFlags },
        { "subpassdescriptionflags", ReservedFlags },
        /* extensions */
        { "surfacetransformflags", SurfaceTransformFlagsKHR },
        { "compositealphaflags", CompositeAlphaFlagsKHR },
        { "displayplanealphaflags", DisplayPlaneAlphaFlagsKHR },
        { "debugreportflags", DebugReportFlagsEXT },
        { "surfacecounterflags", SurfaceCounterFlagsEXT },
        { "externalmemoryhandletypeflags", ExternalMemoryHandleTypeFlags },
        { "externalmemoryfeatureflags", ExternalMemoryFeatureFlags },
        { "externalsemaphorehandletypeflags", ExternalSemaphoreHandleTypeFlags },
        { "externalsemaphorefeatureflags", ExternalSemaphoreFeatureFlags },
        { "semaphoreimportflags", SemaphoreImportFlags },
        { "externalfencehandletypeflags", ExternalFenceHandleTypeFlags },
        { "externalfencefeatureflags", ExternalFenceFeatureFlags },
        { "fenceimportflags", FenceImportFlags },
        { "debugutilsmessageseverityflags", DebugUtilsMessageSeverityFlags },
        { "debugutilsmessagetypeflags", DebugUtilsMessageTypeFlags },
        { "devicegrouppresentmodeflags", DeviceGroupPresentModeFlags },
        { "swapchaincreateflags", SwapchainCreateFlags },
        { "descriptorbindingflags", DescriptorBindingFlags },
        { "conditionalrenderingflags", ConditionalRenderingFlags },
        /* extensions, reserved */
        { "displaymodecreateflags", ReservedFlags }, /* VkDisplayModeCreateFlagsKHR */
        { "displaysurfacecreateflags", ReservedFlags }, /* VkDisplaySurfaceCreateFlagsKHR */
        { "xlibsurfacecreateflags", ReservedFlags }, /* VkXlibSurfaceCreateFlagsKHR */
        { "xcbsurfacecreateflags", ReservedFlags }, /* VkXcbSurfaceCreateFlagsKHR */
        { "waylandsurfacecreateflags", ReservedFlags }, /* VkWaylandSurfaceCreateFlagsKHR */
        { "mirsurfacecreateflags", ReservedFlags }, /* VkMirSurfaceCreateFlagsKHR */
        { "androidsurfacecreateflags", ReservedFlags }, /* VkAndroidSurfaceCreateFlagsKHR */
        { "win32surfacecreateflags", ReservedFlags }, /* VkWin32SurfaceCreateFlagsKHR */
        { "commandpooltrimflags", ReservedFlags }, /* VkCommandPoolTrimFlags */
        { "descriptorupdatetemplatecreateflags", ReservedFlags }, /* VkDescriptorUpdateTemplateCreateFlags */
        { "pipelinediscardrectanglestatecreateflags", ReservedFlags }, /* VkPipelineDiscardRectangleStateCreateFlagsEXT */
        { "validationcachecreateflags", ReservedFlags }, /* VkValidationCacheCreateFlagsEXT */
        { "debugutilsmessengercallbackdataflags", ReservedFlags }, /* VkDebugUtilsMessengerCallbackDataFlagsEXT */
        { "debugutilsmessengercreateflags", ReservedFlags }, /* VkDebugUtilsMessengerCreateFlagsEXT */
        { "pipelinerasterizationconservativestatecreateflags", ReservedFlags }, /* VkPipelineRasterizationConservativeStateCreateFlagsEXT */
        { "pipelinerasterizationstatestreamcreateflags", ReservedFlags }, /* VkPipelineRasterizationStateStreamCreateFlagsEXT */
//      { "", ReservedFlags }, /*  */
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_flags(lua_State *L)
    {
    AddConstants(L);
    luaL_setfuncs(L, Functions, 0);
    }


#if 0 // scaffolding

/*----------------------------------------------------------------------*
 | VkZzzFlagsKHR
 *----------------------------------------------------------------------*/

static VkFlags checkzzzflags(lua_State *L, int arg)
    {
    const char *s;
    VkFlags flags = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(VK_ZZZ_, "");
#undef CASE
        return (VkFlags)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushzzzflags(lua_State *L, VkFlags flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(VK_ZZZ_, "");
#undef CASE

    return n;
    }

static int ZzzFlagsKHR(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushzzzflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkzzzflags(L, 1));
    return 1;
    }

    Add_ZzzFlagsKHR(L);
        { "zzzflags", ZzzFlagsKHR },
#define Add_ZzzFlagsKHR(L) \
    ADD(ZZZ_);\

[[zzzflags]]
[small]#*zzzflags*: vk.ZZZ_XXX_BIT_KHR +
Values:
Rfr: https://www.khronos.org/registry/vulkan/specs/1.0-extensions/html/vkspec.html#VkZzzFlagBitsKHR[VkZzzFlagBitsKHR].#

#endif


