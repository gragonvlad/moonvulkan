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

typedef struct {
    char *memp;    /* start of mapped area (=NULL if not mapped) */
    size_t memsz;  /* size of mapped area (=0 if not mapped) */
    size_t maxsz; /* max size (allocationSize) */
} ud_info_t;


static int freedevice_memory(lua_State *L, ud_t *ud)
    {
    VkDeviceMemory device_memory = (VkDeviceMemory)ud->handle;
    VkDevice device = ud->device;
    const VkAllocationCallbacks *allocator = ud->allocator;

    if(!freeuserdata(L, ud))
        return 0; /* double call */
    TRACE_DELETE(device_memory, "device_memory");
    UD(device)->ddt->FreeMemory(device, device_memory, allocator);
    return 0;
    }


static int Create(lua_State *L)
    {
    ud_info_t *ud_info;
    ud_t *ud, *device_ud;
    VkResult ec;
    VkDeviceMemory device_memory;
    VkMemoryAllocateInfo info;
    VkDevice device = checkdevice(L, 1, &device_ud);
    const VkAllocationCallbacks *allocator = optallocator(L, 4);
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = NULL;
    info.allocationSize = luaL_checkinteger(L, 2);
    info.memoryTypeIndex = luaL_checkinteger(L, 3);

    ud_info = (ud_info_t*)MallocNoErr(L, sizeof(ud_info_t));
    if(!ud_info) return errmemory(L);

    ec = device_ud->ddt->AllocateMemory(device, &info, NULL, &device_memory);
    if(ec)
        {
        Free(L, ud_info);
        CheckError(L, ec);
        }
    TRACE_CREATE(device_memory, "device_memory");
    ud = newuserdata_nondispatchable(L, device_memory, DEVICE_MEMORY_MT);
    ud->parent_ud = device_ud;
    ud->device = device;
    ud->instance = device_ud->instance;
    ud->destructor = freedevice_memory;
    ud->allocator = allocator;
    ud->ddt = device_ud->ddt;
    ud->info = ud_info;
    ud_info->maxsz = info.allocationSize;
    return 1;
    }

static int MapMemory(lua_State *L)
    {
    VkResult ec;
    void *data;
    ud_t *ud;
    VkDeviceMemory memory = checkdevice_memory(L, 1, &ud);
    ud_info_t *ud_info = (ud_info_t*)ud->info;
    VkDevice device = ud->device;
    VkDeviceSize offset = luaL_checkinteger(L, 2);
    VkDeviceSize size = checksizeorwholesize(L, 3);
    VkMemoryMapFlags flags = optflags(L, 4, 0);

    if(ud_info->memp)
        return luaL_error(L, "memory is already mapped");

    ec = ud->ddt->MapMemory(device, memory, offset, size, flags, &data);
    CheckError(L, ec);
    ud_info->memp = (char*)data;
    ud_info->memsz = (size == VK_WHOLE_SIZE) ? ud_info->maxsz - offset : size;
    return 0;
    }

static int UnmapMemory(lua_State *L)
    {
    ud_t *ud;
    VkDeviceMemory memory = checkdevice_memory(L, 1, &ud);
    ud_info_t *ud_info = (ud_info_t*)ud->info;
    VkDevice device = ud->device;

    if(ud_info->memp == NULL)
        return luaL_error(L, "memory is not mapped");

    ud->ddt->UnmapMemory(device, memory);
    ud_info->memp = NULL;
    ud_info->memsz = 0;
    return 0;
    }

static int GetDeviceMemoryCommitment(lua_State *L)
    {
    VkDeviceSize bytes;
    ud_t *ud;
    VkDeviceMemory memory = checkdevice_memory(L, 1, &ud);
    VkDevice device = ud->device;
    ud->ddt->GetDeviceMemoryCommitment(device, memory, &bytes);
    lua_pushinteger(L, bytes);
    return 1;
    }

static int Write(lua_State *L) /* NONVK */
    {
    size_t size;
    ud_t *ud;
    ud_info_t *ud_info;
    VkDeviceSize offset;
    const char *data;

    (void)checkdevice_memory(L, 1, &ud);
    ud_info = (ud_info_t*)ud->info;
    offset = luaL_checkinteger(L, 2);
    data = luaL_tolstring(L, 3, &size);
    if(!ud_info->memp)
        return luaL_error(L, "memory is not mapped");
    /* boundary checks */
    if(size > (ud_info->memsz - offset))
        return luaL_argerror(L, 3, errstring(ERR_LENGTH));
    memcpy(ud_info->memp + offset, data, size);
    return 0;
    }


static int Read(lua_State *L) /* NONVK */
    {
    ud_t *ud;
    ud_info_t *ud_info;
    VkDeviceSize offset, size;

    (void)checkdevice_memory(L, 1, &ud);
    ud_info = (ud_info_t*)ud->info;
    offset = luaL_checkinteger(L, 2);
    size = checksizeorwholesize(L, 3);
    if(!ud_info->memp)
        return luaL_error(L, "memory is not mapped");
    /* boundary checks */
    if(size == VK_WHOLE_SIZE)
        size = ud_info->memsz - offset;
    else if(size > (ud_info->memsz - offset))
        return luaL_argerror(L, 3, errstring(ERR_LENGTH));
    lua_pushlstring(L, ud_info->memp + offset, size);
    return 1;
    }

static int FlushMappedMemoryRanges(lua_State *L)
    {
    int err;
    uint32_t count;
    VkResult ec;
    ud_t *ud;
    VkDevice device = checkdevice(L, 1, &ud);
    VkMappedMemoryRange* ranges = echeckmappedmemoryrangelist(L, 2, &count, &err);
    if(err) return luaL_argerror(L, 2, errstring(err));
    
    ec = ud->ddt->FlushMappedMemoryRanges(device, count, ranges);
    Free(L, ranges);
    CheckError(L, ec);
    return 0;
    }

static int InvalidateMappedMemoryRanges(lua_State *L)
    {
    int err;
    uint32_t count;
    VkResult ec;
    ud_t *ud;
    VkDevice device = checkdevice(L, 1, &ud);
    VkMappedMemoryRange* ranges = echeckmappedmemoryrangelist(L, 2, &count, &err);
    if(err) return luaL_argerror(L, 2, errstring(err));
    
    ec = ud->ddt->InvalidateMappedMemoryRanges(device, count, ranges);
    Free(L, ranges);
    CheckError(L, ec);
    return 0;
    }

RAW_FUNC(device_memory)
TYPE_FUNC(device_memory)
INSTANCE_FUNC(device_memory)
DEVICE_FUNC(device_memory)
PARENT_FUNC(device_memory)
DELETE_FUNC(device_memory)
DESTROY_FUNC(device_memory)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "type", Type },
        { "instance", Instance },
        { "device", Device },
        { "parent", Parent },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "allocate_memory",  Create },
        { "free_memory",  Destroy },
        { "map_memory", MapMemory },
        { "unmap_memory", UnmapMemory },
        { "get_device_memory_commitment", GetDeviceMemoryCommitment },
        { "flush_mapped_memory_ranges", FlushMappedMemoryRanges },
        { "invalidate_mapped_memory_ranges", InvalidateMappedMemoryRanges },
        { "write_memory", Write },
        { "read_memory", Read },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_device_memory(lua_State *L)
    {
    udata_define(L, DEVICE_MEMORY_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

