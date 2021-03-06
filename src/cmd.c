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
    
static int CmdBindPipeline(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkPipelineBindPoint pipelineBindPoint = checkpipelinebindpoint(L, 2);
    VkPipeline pipeline = checkpipeline(L, 3, NULL);
    ud->ddt->CmdBindPipeline(cb, pipelineBindPoint, pipeline);
    return 0;
    }

static int CmdSetViewport(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t firstViewport = luaL_checkinteger(L, 2);
#define CLEANUP zfreearrayVkViewport (L, viewports, count, 1)
    VkViewport *viewports = zcheckarrayVkViewport(L, 3, &count, &err);
    if(err) { CLEANUP; return argerror(L, 3); }
    ud->ddt->CmdSetViewport(cb, firstViewport, count, viewports);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdSetScissor(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t first = luaL_checkinteger(L, 2);
#define CLEANUP zfreearrayVkRect2D(L, scissors, count, 1)
    VkRect2D* scissors = zcheckarrayVkRect2D(L, 3, &count, &err);
    if(err) { CLEANUP; return argerror(L, 3); }
    ud->ddt->CmdSetScissor(cb, first, count, scissors);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdSetLineWidth(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    float line_width = luaL_optnumber(L, 2, 1.0);
    ud->ddt->CmdSetLineWidth(cb, line_width);
    return 0;
    }

static int CmdSetDepthBias(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    float depthBiasConstantFactor = luaL_checknumber(L, 2);
    float depthBiasClamp = luaL_checknumber(L, 3); 
    float depthBiasSlopeFactor = luaL_checknumber(L, 4);
    ud->ddt->CmdSetDepthBias(cb, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
    return 0;
    }

static int CmdSetBlendConstants(lua_State *L)
    {
    float blendConstants[4];
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    blendConstants[0] = luaL_checknumber(L, 2);
    blendConstants[1] = luaL_checknumber(L, 3);
    blendConstants[2] = luaL_checknumber(L, 4);
    blendConstants[3] = luaL_checknumber(L, 5);
    ud->ddt->CmdSetBlendConstants(cb, blendConstants);
    return 0;
    }

static int CmdSetDepthBounds(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    float minDepthBounds = luaL_checknumber(L, 2);
    float maxDepthBounds = luaL_checknumber(L, 3);
    ud->ddt->CmdSetDepthBounds(cb, minDepthBounds, maxDepthBounds);
    return 0;
    }

static int CmdSetStencilCompareMask(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkStencilFaceFlags faceMask = checkflags(L, 2);
    uint32_t compareMask = luaL_checkinteger(L, 3);
    ud->ddt->CmdSetStencilCompareMask(cb, faceMask, compareMask);
    return 0;
    }

static int CmdSetStencilWriteMask(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkStencilFaceFlags faceMask = checkflags(L, 2);
    uint32_t writeMask = luaL_checkinteger(L, 3);
    ud->ddt->CmdSetStencilWriteMask(cb, faceMask, writeMask);
    return 0;
    }

static int CmdSetStencilReference(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkStencilFaceFlags faceMask = checkflags(L, 2);
    uint32_t reference = luaL_checkinteger(L, 3);
    ud->ddt->CmdSetStencilReference(cb, faceMask, reference);
    return 0;
    }

static int CmdBindDescriptorSets(lua_State *L)
    {
    int err; 
    uint32_t sets_count, offsets_count = 0;
    VkDescriptorSet* sets; 
    uint32_t* offsets;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkPipelineBindPoint bindpoint = checkpipelinebindpoint(L, 2);
    VkPipelineLayout layout = checkpipeline_layout(L, 3, NULL);
    uint32_t firstSet = luaL_checkinteger(L, 4);

    sets = checkdescriptor_setlist(L, 5, &sets_count, &err, NULL);
    if(err) return argerrorc(L, 5, err);
    
    offsets = checkuint32list(L, 6, &offsets_count, &err);
    if(err < 0) { Free(L, sets); return argerrorc(L, 6, err); }

    ud->ddt->CmdBindDescriptorSets(cb, bindpoint, layout, firstSet, sets_count, sets, offsets_count, offsets);
    Free(L, sets); 
    if(offsets) Free(L, offsets);
    return 0;
    }

static int CmdBindIndexBuffer(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer buffer = checkbuffer(L, 2, NULL);
    VkDeviceSize offset = luaL_checkinteger(L, 3);
    VkIndexType indexType = checkindextype(L, 4);
    ud->ddt->CmdBindIndexBuffer(cb, buffer, offset, indexType);
    return 0;
    }

static int CmdBindVertexBuffers(lua_State *L)
    {
    int err;
    uint32_t count, offsets_count;
    VkBuffer* buffers;
    VkDeviceSize* offsets;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t first = luaL_checkinteger(L, 2);

    buffers = checkbufferlist(L, 3, &count, &err);
    if(err) return argerrorc(L, 3, err);
    
    offsets = checkdevicesizelist(L, 4, &offsets_count, &err);
    if(err)
        { 
        Free(L, buffers); 
        return argerrorc(L, 4, err);
        }
    if(offsets_count != count)
        { 
        Free(L, buffers); 
        Free(L, offsets); 
        return argerrorc(L, 4, ERR_LENGTH);
        }

    ud->ddt->CmdBindVertexBuffers(cb, first, count, buffers, offsets);
    Free(L, buffers);
    Free(L, offsets);
    return 0;
    }

static int CmdDraw(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t vertexCount = luaL_checkinteger(L, 2);
    uint32_t instanceCount = luaL_checkinteger(L, 3);
    uint32_t firstVertex = luaL_checkinteger(L, 4);
    uint32_t firstInstance = luaL_checkinteger(L, 5);
    ud->ddt->CmdDraw(cb, vertexCount, instanceCount, firstVertex, firstInstance);
    return 0;
    }

static int CmdDrawIndexed(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t indexCount = luaL_checkinteger(L, 2);
    uint32_t instanceCount = luaL_checkinteger(L, 3);
    uint32_t firstIndex = luaL_checkinteger(L, 4);
    int32_t vertexOffset = luaL_checkinteger(L, 5);
    uint32_t firstInstance = luaL_checkinteger(L, 6);
    ud->ddt->CmdDrawIndexed(cb, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    return 0;
    }

static int CmdDrawIndirect(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer buffer = checkbuffer(L, 2, NULL);
    VkDeviceSize offset = luaL_checkinteger(L, 3);
    uint32_t drawCount = luaL_checkinteger(L, 4);
    uint32_t stride = luaL_checkinteger(L, 5);
    ud->ddt->CmdDrawIndirect(cb, buffer, offset, drawCount, stride);
    return 0;
    }

static int CmdDrawIndexedIndirect(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer buffer = checkbuffer(L, 2, NULL);
    VkDeviceSize offset = luaL_checkinteger(L, 3);
    uint32_t drawCount = luaL_checkinteger(L, 4);
    uint32_t stride = luaL_checkinteger(L, 5);
    ud->ddt->CmdDrawIndexedIndirect(cb, buffer, offset, drawCount, stride);
    return 0;
    }

static int CmdDispatch(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t x = luaL_checkinteger(L, 2);
    uint32_t y = luaL_checkinteger(L, 3);
    uint32_t z = luaL_checkinteger(L, 4);
    ud->ddt->CmdDispatch(cb, x, y, z);
    return 0;
    }

static int CmdDispatchIndirect(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer buffer = checkbuffer(L, 2, NULL);
    VkDeviceSize offset = luaL_checkinteger(L, 3);
    ud->ddt->CmdDispatchIndirect(cb, buffer, offset);
    return 0;
    }

static int CmdCopyBuffer(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer srcBuffer = checkbuffer(L, 2, NULL);
    VkBuffer dstBuffer = checkbuffer(L, 3, NULL);
#define CLEANUP zfreearrayVkBufferCopy(L, regions, count, 1)
    VkBufferCopy* regions = zcheckarrayVkBufferCopy(L, 4, &count, &err);
    if(err) { CLEANUP; return argerror(L, 4); }
    ud->ddt->CmdCopyBuffer(cb, srcBuffer, dstBuffer, count, regions);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdCopyImage(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkImage srcImage = checkimage(L, 2, NULL);
    VkImageLayout srcImageLayout = checkimagelayout(L, 3);
    VkImage dstImage = checkimage(L, 4, NULL);
    VkImageLayout dstImageLayout = checkimagelayout(L, 5);
#define CLEANUP zfreearrayVkImageCopy(L, regions, count, 1)
    VkImageCopy* regions = zcheckarrayVkImageCopy(L, 6, &count, &err);
    if(err) { CLEANUP; return argerror(L, 6); }
    ud->ddt->CmdCopyImage(cb, srcImage, srcImageLayout, dstImage, dstImageLayout, count, regions);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdBlitImage(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkImage srcImage = checkimage(L, 2, NULL);
    VkImageLayout srcImageLayout = checkimagelayout(L, 3);
    VkImage dstImage = checkimage(L, 4, NULL);
    VkImageLayout dstImageLayout = checkimagelayout(L, 5);
    VkFilter filter = checkfilter(L, 7); /* before regions so there is no need to free them on error */
#define CLEANUP zfreearrayVkImageBlit(L, regions, count, 1)
    VkImageBlit* regions = zcheckarrayVkImageBlit(L, 6, &count, &err);
    if(err) { CLEANUP; return argerror(L, 6); }
    ud->ddt->CmdBlitImage(cb, srcImage, srcImageLayout, dstImage, dstImageLayout, count, regions, filter);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdCopyBufferToImage(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer srcBuffer = checkbuffer(L, 2, NULL);
    VkImage dstImage = checkimage(L, 3, NULL);
    VkImageLayout dstImageLayout = checkimagelayout(L, 4);
#define CLEANUP zfreearrayVkBufferImageCopy(L, regions, count, 1)
    VkBufferImageCopy* regions = zcheckarrayVkBufferImageCopy(L, 5, &count, &err);
    if(err) { CLEANUP; return argerror(L, 5); }
    ud->ddt->CmdCopyBufferToImage(cb, srcBuffer, dstImage, dstImageLayout, count, regions);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdCopyImageToBuffer(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkImage srcImage = checkimage(L, 2, NULL);
    VkImageLayout srcImageLayout = checkimagelayout(L, 3);
    VkBuffer dstBuffer = checkbuffer(L, 4, NULL);
#define CLEANUP zfreearrayVkBufferImageCopy(L, regions, count, 1)
    VkBufferImageCopy* regions = zcheckarrayVkBufferImageCopy(L, 5, &count, &err);
    if(err) { CLEANUP; return argerror(L, 5); }
    ud->ddt->CmdCopyImageToBuffer(cb, srcImage, srcImageLayout, dstBuffer, count, regions);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdUpdateBuffer(lua_State *L)
    {
    size_t size;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer dstBuffer = checkbuffer(L, 2, NULL);
    VkDeviceSize dstOffset = luaL_checkinteger(L, 3);
    const char *data = luaL_checklstring(L, 4, &size);
    if((size==0) || (size % 4)!=0)
        return argerrorc(L, 4, ERR_LENGTH);
    ud->ddt->CmdUpdateBuffer(cb, dstBuffer, dstOffset, (VkDeviceSize)size, data);
    return 0;
    }

static int CmdFillBuffer(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer dstBuffer = checkbuffer(L, 2, NULL);
    VkDeviceSize dstOffset = luaL_checkinteger(L, 3);
    VkDeviceSize size = checksizeorwholesize(L, 4);
    uint32_t data = luaL_checkinteger(L, 5);
    ud->ddt->CmdFillBuffer(cb, dstBuffer, dstOffset, size, data);
    return 0;
    }

static int CmdClearColorImage(lua_State *L)
    {
    int err;
    ud_t *ud;
    uint32_t count=0;
    VkImageSubresourceRange* ranges=NULL;
    VkClearColorValue* color=NULL;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkImage image = checkimage(L, 2, NULL);
    VkImageLayout imageLayout = checkimagelayout(L, 3);
#define CLEANUP do {                                            \
    zfreeVkClearColorValue(L, color, 1);                        \
    zfreearrayVkImageSubresourceRange(L, ranges, count, 1);     \
} while(0)
    color = zcheckVkClearColorValue(L, 4, &err);
    if(err) { CLEANUP; return argerror(L, 4); }
    ranges = zcheckarrayVkImageSubresourceRange(L, 5, &count, &err);
    if(err) { CLEANUP; return argerror(L, 5); }
    ud->ddt->CmdClearColorImage(cb, image, imageLayout, color, count, ranges);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdClearDepthStencilImage(lua_State *L)
    {
    int err;
    uint32_t count;
    VkClearDepthStencilValue depthstencil;
    VkImageSubresourceRange* ranges;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkImage image = checkimage(L, 2, NULL);
    VkImageLayout imageLayout = checkimagelayout(L, 3);
    depthstencil.depth = luaL_checknumber(L, 4);
    depthstencil.stencil = luaL_checkinteger(L, 5);
#define CLEANUP zfreearrayVkImageSubresourceRange(L, ranges, count, 1)
    ranges = zcheckarrayVkImageSubresourceRange(L, 6, &count, &err);
    if(err) { CLEANUP; return argerror(L, 6); }
    ud->ddt->CmdClearDepthStencilImage(cb, image, imageLayout, &depthstencil, count, ranges);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdClearAttachments(lua_State *L)
    {
    int err;
    ud_t *ud;
    uint32_t att_count=0;
    uint32_t rect_count=0;
    VkClearAttachment* attachments=NULL;
    VkClearRect* rects=NULL;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
#define CLEANUP do {                                            \
    zfreearrayVkClearRect(L, rects, rect_count, 1);             \
    zfreearrayVkClearAttachment(L, attachments, att_count, 1);  \
} while(0)
    attachments = zcheckarrayVkClearAttachment(L, 2, &att_count, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    rects = zcheckarrayVkClearRect(L, 3, &rect_count, &err);
    if(err) { CLEANUP; return argerror(L, 3); }
    ud->ddt->CmdClearAttachments(cb, att_count, attachments, rect_count, rects);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdResolveImage(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkImage srcImage = checkimage(L, 2, NULL);
    VkImageLayout srcImageLayout = checkimagelayout(L, 3);
    VkImage dstImage = checkimage(L, 4, NULL);
    VkImageLayout dstImageLayout = checkimagelayout(L, 5);
#define CLEANUP zfreearrayVkImageResolve(L, regions, count, 1)
    VkImageResolve* regions = zcheckarrayVkImageResolve(L, 6, &count, &err);
    if(err) { CLEANUP; return argerror(L, 6); }
    ud->ddt->CmdResolveImage(cb, srcImage, srcImageLayout, dstImage, dstImageLayout, count, regions);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdSetEvent(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkEvent event = checkevent(L, 2, NULL);
    VkPipelineStageFlags stageMask = checkflags(L, 3);
    ud->ddt->CmdSetEvent(cb, event, stageMask);
    return 0;
    }

static int CmdResetEvent(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkEvent event = checkevent(L, 2, NULL);
    VkPipelineStageFlags stageMask = checkflags(L, 3);
    ud->ddt->CmdResetEvent(cb, event, stageMask);
    return 0;
    }

static int CmdWaitEvents(lua_State *L)
    {
    int err;
    ud_t *ud;
    uint32_t eventCount, mCount=0, bCount=0, iCount=0;
    VkEvent* pEvents = NULL;
    VkMemoryBarrier* pMemoryBarriers = NULL;
    VkBufferMemoryBarrier* pBufferMemoryBarriers = NULL;
    VkImageMemoryBarrier* pImageMemoryBarriers = NULL;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkPipelineStageFlags srcStageMask = checkflags(L, 2);
    VkPipelineStageFlags dstStageMask = checkflags(L, 3);
#define CLEANUP do {                                                        \
    Free(L, pEvents);                                                       \
    zfreearrayVkMemoryBarrier(L, pMemoryBarriers, mCount, 1);               \
    zfreearrayVkBufferMemoryBarrier(L, pBufferMemoryBarriers, bCount, 1);   \
    zfreearrayVkImageMemoryBarrier(L, pImageMemoryBarriers, iCount, 1);     \
} while(0)
    pEvents = checkeventlist(L, 4, &eventCount, &err, NULL);
    if(err) { CLEANUP; return argerrorc(L, 4, err); }

    pMemoryBarriers = zcheckarrayVkMemoryBarrier(L, 5, &mCount, &err);
    if(err < 0) { CLEANUP; return argerror(L, 5); }
    if(err) lua_pop(L, 1);

    pBufferMemoryBarriers = zcheckarrayVkBufferMemoryBarrier(L, 6, &bCount, &err);
    if(err < 0) { CLEANUP; return argerror(L, 6); }
    if(err) lua_pop(L, 1);

    pImageMemoryBarriers = zcheckarrayVkImageMemoryBarrier(L, 7, &iCount, &err);
    if(err < 0) { CLEANUP; return argerror(L, 7); }
    if(err) lua_pop(L, 1);

    ud->ddt->CmdWaitEvents(cb, eventCount, pEvents, srcStageMask, dstStageMask, 
            mCount, pMemoryBarriers, bCount, pBufferMemoryBarriers, 
            iCount, pImageMemoryBarriers);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdPipelineBarrier(lua_State *L)
    {
    int err;
    ud_t *ud;
    uint32_t mCount=0, bCount=0, iCount=0;
    VkMemoryBarrier* pMemoryBarriers = NULL;
    VkBufferMemoryBarrier* pBufferMemoryBarriers = NULL;
    VkImageMemoryBarrier* pImageMemoryBarriers = NULL;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkPipelineStageFlags srcStageMask = checkflags(L, 2);
    VkPipelineStageFlags dstStageMask = checkflags(L, 3);
    VkDependencyFlags dependencyFlags = checkflags(L, 4);
#define CLEANUP do {                                                        \
    zfreearrayVkMemoryBarrier(L, pMemoryBarriers, mCount, 1);               \
    zfreearrayVkBufferMemoryBarrier(L, pBufferMemoryBarriers, bCount, 1);   \
    zfreearrayVkImageMemoryBarrier(L, pImageMemoryBarriers, iCount, 1);     \
} while(0)
    pMemoryBarriers = zcheckarrayVkMemoryBarrier(L, 5, &mCount, &err);
    if(err < 0) { CLEANUP; return argerror(L, 5); }
    if(err) lua_pop(L, 1);

    pBufferMemoryBarriers = zcheckarrayVkBufferMemoryBarrier(L, 6, &bCount, &err);
    if(err < 0) { CLEANUP; return argerror(L, 6); }
    if(err) lua_pop(L, 1);

    pImageMemoryBarriers = zcheckarrayVkImageMemoryBarrier(L, 7, &iCount, &err);
    if(err < 0) { CLEANUP; return argerror(L, 7); }
    if(err) lua_pop(L, 1);

    ud->ddt->CmdPipelineBarrier(cb, srcStageMask, dstStageMask, dependencyFlags, 
        mCount, pMemoryBarriers, bCount, pBufferMemoryBarriers, 
        iCount, pImageMemoryBarriers);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdBeginQuery(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkQueryPool queryPool = checkquery_pool(L, 2, NULL);
    uint32_t query = luaL_checkinteger(L, 3);
    VkQueryControlFlags flags = checkflags(L, 4);
    ud->ddt->CmdBeginQuery(cb, queryPool, query, flags);
    return 0;
    }

static int CmdEndQuery(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkQueryPool queryPool = checkquery_pool(L, 2, NULL);
    uint32_t query = luaL_checkinteger(L, 3);
    ud->ddt->CmdEndQuery(cb, queryPool, query);
    return 0;
    }

static int CmdResetQueryPool(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkQueryPool queryPool = checkquery_pool(L, 2, NULL);
    uint32_t firstQuery = luaL_checkinteger(L, 3);
    uint32_t queryCount = luaL_checkinteger(L, 4);
    ud->ddt->CmdResetQueryPool(cb, queryPool, firstQuery, queryCount);
    return 0;
    }

static int CmdWriteTimestamp(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkPipelineStageFlagBits pipelineStage = (VkPipelineStageFlagBits)checkflags(L, 2);
    VkQueryPool queryPool = checkquery_pool(L, 3, NULL);
    uint32_t query = luaL_checkinteger(L, 4);
    ud->ddt->CmdWriteTimestamp(cb, pipelineStage, queryPool, query);
    return 0;
    }

static int CmdCopyQueryPoolResults(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkQueryPool queryPool = checkquery_pool(L, 2, NULL);
    uint32_t firstQuery = luaL_checkinteger(L, 3);
    uint32_t queryCount = luaL_checkinteger(L, 4);
    VkBuffer dstBuffer = checkbuffer(L, 5, NULL);
    VkDeviceSize dstOffset  = luaL_checkinteger(L, 6);
    VkDeviceSize stride = luaL_checkinteger(L, 7);
    VkQueryResultFlags flags = checkflags(L, 8);
    ud->ddt->CmdCopyQueryPoolResults(cb, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
    return 0;
    }

static int CmdPushConstants(lua_State *L)
    {
    size_t size;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkPipelineLayout layout = checkpipeline_layout(L, 2, NULL);
    VkShaderStageFlags stageFlags = checkflags(L, 3);
    uint32_t offset = luaL_checkinteger(L, 4);
    const char* values = luaL_checklstring(L, 5, &size);
    ud->ddt->CmdPushConstants(cb, layout, stageFlags, offset, (uint32_t)size, values);
    return 0;
    }

static int CmdBeginRenderPass(lua_State *L)
    {
    int err;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    if(ud->ddt->CmdBeginRenderPass2KHR)
        {
#define CLEANUP do {                                    \
            zfreeVkRenderPassBeginInfo(L, info, 1);     \
            zfreeVkSubpassBeginInfoKHR(L, binfo, 1);    \
} while(0)
        VkRenderPassBeginInfo* info=NULL;
        VkSubpassBeginInfoKHR* binfo=NULL;
        info = zcheckVkRenderPassBeginInfo(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        binfo = zcheckVkSubpassBeginInfoKHR(L, 3, &err);
        if(err) { CLEANUP; return argerror(L, 3); }
        ud->ddt->CmdBeginRenderPass2KHR(cb, info, binfo);
        CLEANUP;
#undef CLEANUP
        }
    else
        {
        VkSubpassContents contents = checksubpasscontents(L, 3);
#define CLEANUP zfreeVkRenderPassBeginInfo(L, info, 1)
        VkRenderPassBeginInfo* info = zcheckVkRenderPassBeginInfo(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        ud->ddt->CmdBeginRenderPass(cb, info, contents);
        CLEANUP;
#undef CLEANUP
        }
    return 0;
    }

static int CmdNextSubpass(lua_State *L)
    {
    int err;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    if(ud->ddt->CmdNextSubpass2KHR)
        {
#define CLEANUP do {                                    \
            zfreeVkSubpassBeginInfoKHR(L, binfo, 1);    \
            zfreeVkSubpassEndInfoKHR(L, einfo, 1);      \
} while(0)
        VkSubpassBeginInfoKHR* binfo = NULL;
        VkSubpassEndInfoKHR* einfo = NULL;
        binfo = zcheckVkSubpassBeginInfoKHR(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        einfo = zcheckVkSubpassEndInfoKHR(L, 3, &err);
        if(err) { CLEANUP; return argerror(L, 3); }
        ud->ddt->CmdNextSubpass2KHR(cb, binfo, einfo);
        CLEANUP;
#undef CLEANUP
        }
    else
        {
        VkSubpassContents contents = checksubpasscontents(L, 2);
        ud->ddt->CmdNextSubpass(cb, contents);
        }
    return 0;
    }

static int CmdEndRenderPass(lua_State *L)
    {
    int err;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    if(ud->ddt->CmdEndRenderPass2KHR)
        {
#define CLEANUP zfreeVkSubpassEndInfoKHR(L, einfo, 1)
        VkSubpassEndInfoKHR* einfo;
        einfo = zcheckVkSubpassEndInfoKHR(L, 2, &err);
        if(err) { CLEANUP; return argerror(L, 2); }
        ud->ddt->CmdEndRenderPass2KHR(cb, einfo);
        CLEANUP;
#undef CLEANUP
        }
    else
        {
        ud->ddt->CmdEndRenderPass(cb);
        }
    return 0;
    }

static int CmdExecuteCommands(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkCommandBuffer *buffers = checkcommand_bufferlist(L, 2, &count, &err);
    if(err) return argerrorc(L, 2, err);

    ud->ddt->CmdExecuteCommands(cb, count, buffers);
    Free(L, buffers);
    return 0;
    }


/*-------- Extensions ------------------------------------------------------------ */

static int checkdebugmarkermarkerinfo(lua_State *L, int arg, VkDebugMarkerMarkerInfoEXT *info)
    {
    int i, argcolor = arg+1;
    info->sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
    info->pNext = NULL;
    info->pMarkerName = luaL_checkstring(L, arg);
    if(lua_isnoneornil(L, argcolor))
        {
        memset(info->color, 0, 4*sizeof(float));
        return 0;
        }
    if(!lua_istable(L, argcolor))
        return argerrorc(L, arg, ERR_TABLE);
    for(i = 0; i < 4; i++)
        {
        if(lua_rawgeti(L, argcolor, i+1) != LUA_TNUMBER)
            {
            lua_pop(L, 1);
            return argerrorc(L, argcolor, ERR_TYPE);
            }
        info->color[i] = lua_tonumber(L, -1);
        lua_pop(L, 1);
        }
    return 0;
    }

static int CmdDebugMarkerBegin(lua_State *L) 

    {
    ud_t *ud;
    VkDebugMarkerMarkerInfoEXT info;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    checkdebugmarkermarkerinfo(L, 2, &info);
    CheckDevicePfn(L, ud, CmdDebugMarkerBeginEXT);
    ud->ddt->CmdDebugMarkerBeginEXT(cb, &info);
    return 0;
    }

static int CmdDebugMarkerEnd(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdDebugMarkerEndEXT);
    ud->ddt->CmdDebugMarkerEndEXT(cb);
    return 0;
    }

static int CmdDebugMarkerInsert(lua_State *L)
    {
    ud_t *ud;
    VkDebugMarkerMarkerInfoEXT info;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdDebugMarkerInsertEXT);
    checkdebugmarkermarkerinfo(L, 2, &info);
    ud->ddt->CmdDebugMarkerInsertEXT(cb, &info);
    return 0;
    }

static int CmdPushDescriptorSet(lua_State *L)
    {
    int err;
    ud_t *ud;
    uint32_t count;
    VkWriteDescriptorSet* writes;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkPipelineBindPoint pipelineBindPoint = checkpipelinebindpoint(L, 2);
    VkPipelineLayout layout = checkpipeline_layout(L, 3, NULL);
    uint32_t set = luaL_checkinteger(L, 4);
    CheckDevicePfn(L, ud, CmdPushDescriptorSetKHR);
#define CLEANUP zfreearrayVkWriteDescriptorSet(L, writes, count, 1)
    writes = zcheckarrayVkWriteDescriptorSet(L, 5, &count, &err);
    if(err) { CLEANUP; return argerror(L, 5); }
    ud->ddt->CmdPushDescriptorSetKHR(cb, pipelineBindPoint, layout, set, count, writes);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdPushDescriptorSetWithTemplate(lua_State *L)
    {
    size_t len;
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkDescriptorUpdateTemplate desc_template = checkdescriptor_update_template(L, 2, NULL);
    VkPipelineLayout layout = checkpipeline_layout(L, 3, NULL);
    uint32_t set = luaL_checkinteger(L, 4);
    const void* data = luaL_checklstring(L, 5, &len);
    CheckDevicePfn(L, ud, CmdPushDescriptorSetWithTemplateKHR);
    ud->ddt->CmdPushDescriptorSetWithTemplateKHR(cb, desc_template, layout, set, data);
    return 0;
    }

static int CmdSetDiscardRectangle(lua_State *L)
    {
    int err;
    uint32_t count;
    ud_t *ud;
    VkRect2D* rects;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t first = luaL_checkinteger(L, 2);
    CheckDevicePfn(L, ud, CmdSetDiscardRectangleEXT);
#define CLEANUP zfreearrayVkRect2D(L, rects, count, 1)
    rects = zcheckarrayVkRect2D(L, 3, &count, &err);
    if(err) { CLEANUP; return argerror(L, 3); }
    ud->ddt->CmdSetDiscardRectangleEXT(cb, first, count, rects);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdSetSampleLocations(lua_State *L)
    {
    int err;
    ud_t *ud;
    VkSampleLocationsInfoEXT* info;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdSetSampleLocationsEXT);
#define CLEANUP zfreeVkSampleLocationsInfoEXT(L, info, 1)
    info = zcheckVkSampleLocationsInfoEXT(L, 2, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    ud->ddt->CmdSetSampleLocationsEXT(cb, info);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdBeginDebugUtilsLabel(lua_State *L)
    {
    int err;
    ud_t *ud;
    VkDebugUtilsLabelEXT* label;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckInstancePfn(L, ud, CmdBeginDebugUtilsLabelEXT);
#define CLEANUP zfreeVkDebugUtilsLabelEXT(L, label, 1)
    label = zcheckVkDebugUtilsLabelEXT(L, 2, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    ud->idt->CmdBeginDebugUtilsLabelEXT(cb, label);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdEndDebugUtilsLabel(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckInstancePfn(L, ud, CmdEndDebugUtilsLabelEXT);
    ud->idt->CmdEndDebugUtilsLabelEXT(cb);
    return 0;
    }

static int CmdInsertDebugUtilsLabel(lua_State *L)
    {
    int err;
    ud_t *ud;
    VkDebugUtilsLabelEXT* label;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckInstancePfn(L, ud, CmdInsertDebugUtilsLabelEXT);
#define CLEANUP zfreeVkDebugUtilsLabelEXT(L, label, 1)
    label = zcheckVkDebugUtilsLabelEXT(L, 2, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    ud->idt->CmdInsertDebugUtilsLabelEXT(cb, label);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdBeginConditionalRendering(lua_State *L)
    {
    int err;
    ud_t *ud;
    VkConditionalRenderingBeginInfoEXT* info;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
#define CLEANUP zfreeVkConditionalRenderingBeginInfoEXT(L, info, 1)
    info = zcheckVkConditionalRenderingBeginInfoEXT(L, 2, &err);
    if(err) { CLEANUP; return argerror(L, 2); }
    CheckDevicePfn(L, ud, CmdBeginConditionalRenderingEXT);
    ud->ddt->CmdBeginConditionalRenderingEXT(cb, info);
    CLEANUP;
#undef CLEANUP
    return 0;
    }

static int CmdEndConditionalRendering(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdEndConditionalRenderingEXT);
    ud->ddt->CmdEndConditionalRenderingEXT(cb);
    return 0;
    }

static int CmdDrawIndirectCount(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer buffer = checkbuffer(L, 2, NULL);
    VkDeviceSize offset = luaL_checkinteger(L, 3);
    VkBuffer countBuffer = checkbuffer(L, 4, NULL);
    VkDeviceSize countBufferOffset = luaL_checkinteger(L, 5);
    uint32_t maxDrawCount = luaL_checkinteger(L, 6);
    uint32_t stride = luaL_checkinteger(L, 7);
    CheckDevicePfn(L, ud, CmdDrawIndirectCountKHR);
    ud->ddt->CmdDrawIndirectCountKHR(cb, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    return 0;
    }

static int CmdDrawIndexedIndirectCount(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    VkBuffer buffer = checkbuffer(L, 2, NULL);
    VkDeviceSize offset = luaL_checkinteger(L, 3);
    VkBuffer countBuffer = checkbuffer(L, 4, NULL);
    VkDeviceSize countBufferOffset = luaL_checkinteger(L, 5);
    uint32_t maxDrawCount = luaL_checkinteger(L, 6);
    uint32_t stride = luaL_checkinteger(L, 7);
    CheckDevicePfn(L, ud, CmdDrawIndexedIndirectCountKHR);
    ud->ddt->CmdDrawIndexedIndirectCountKHR(cb, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    return 0;
    }

static int CmdSetDeviceMask(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t deviceMask = luaL_checkinteger(L, 2);
    CheckDevicePfn(L, ud, CmdSetDeviceMaskKHR);
    ud->ddt->CmdSetDeviceMaskKHR(cb, deviceMask);
    return 0;
    }

static int CmdDispatchBase(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t baseGroupX = luaL_checkinteger(L, 2);
    uint32_t baseGroupY = luaL_checkinteger(L, 3);
    uint32_t baseGroupZ = luaL_checkinteger(L, 4);
    uint32_t groupCountX = luaL_checkinteger(L, 5);
    uint32_t groupCountY = luaL_checkinteger(L, 6);
    uint32_t groupCountZ = luaL_checkinteger(L, 7);
    CheckDevicePfn(L, ud, CmdDispatchBaseKHR);
    ud->ddt->CmdDispatchBaseKHR(cb, baseGroupX, baseGroupY, baseGroupZ,
            groupCountX, groupCountY, groupCountZ);
    return 0;
    }

static int CmdBindTransformFeedbackBuffers(lua_State *L)
    {
    int err = 0, errargc=1;
    ud_t *ud;
    uint32_t first_binding, count, count1;
    VkBuffer* buffers = NULL;
    VkDeviceSize *offsets = NULL;
    VkDeviceSize *sizes = NULL;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdBindTransformFeedbackBuffersEXT);

    first_binding = luaL_checkinteger(L, 2);

    buffers = checkbufferlist(L, 3, &count, &err);
    if(err) { errargc=3; goto failure; }

    offsets = checkdevicesizelist(L, 4, &count1, &err);
    if(err == ERR_NOTPRESENT) err = 0;
    else if(err) { errargc=4; goto failure; }
    if(count1 != count) { err=ERR_LENGTH; errargc=4; goto failure; }

    sizes = checkdevicesizelist(L, 5, &count1, &err);
    if(err == ERR_NOTPRESENT) err = 0;
    else if(err) { errargc=5; goto failure; }
    else if(count1 != count) { err=ERR_LENGTH; errargc=5; goto failure; }

    ud->ddt->CmdBindTransformFeedbackBuffersEXT(cb, first_binding, count, buffers, offsets, sizes);
failure:
    if(buffers) Free(L, buffers);
    if(offsets) Free(L, offsets);
    if(sizes) Free(L, sizes);
    return err ? argerrorc(L, errargc, err) : 0;
    }


static int CmdBeginTransformFeedback(lua_State *L)
    {
    ud_t *ud;
    int err = 0, errargc=1;
    uint32_t first, count, count1;
    VkBuffer* buffers = NULL;
    VkDeviceSize *offsets = NULL;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdBeginTransformFeedbackEXT);

    first = luaL_checkinteger(L, 2);

    buffers = checkbufferlist(L, 3, &count, &err);
    if(err == ERR_NOTPRESENT) err = 0;
    else if(err) { errargc=3; goto failure; }

    offsets = checkdevicesizelist(L, 4, &count1, &err);
    if(err == ERR_NOTPRESENT) err = 0;
    else if(err) { errargc=4; goto failure; }
    if(count1 != count) { err=ERR_LENGTH; errargc=4; goto failure; }

    ud->ddt->CmdBeginTransformFeedbackEXT(cb, first, count, buffers, offsets);
failure:
    if(buffers) Free(L, buffers);
    if(offsets) Free(L, offsets);
    return err ? argerrorc(L, errargc, err) : 0;
    }

static int CmdEndTransformFeedback(lua_State *L)
    {
    ud_t *ud;
    int err = 0, errargc=1;
    uint32_t first, count, count1;
    VkBuffer* buffers = NULL;
    VkDeviceSize *offsets = NULL;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdEndTransformFeedbackEXT);

    first = luaL_checkinteger(L, 2);

    buffers = checkbufferlist(L, 3, &count, &err);
    if(err == ERR_NOTPRESENT) err = 0;
    else if(err) { errargc=3; goto failure; }

    offsets = checkdevicesizelist(L, 4, &count1, &err);
    if(err == ERR_NOTPRESENT) err = 0;
    else if(err) { errargc=4; goto failure; }
    if(count1 != count) { err=ERR_LENGTH; errargc=4; goto failure; }

    ud->ddt->CmdEndTransformFeedbackEXT(cb, first, count, buffers, offsets);
failure:
    if(buffers) Free(L, buffers);
    if(offsets) Free(L, offsets);
    return err ? argerrorc(L, errargc, err) : 0;
    }

static int CmdBeginQueryIndexed(lua_State *L)
    {
    ud_t *ud;
    VkQueryPool query_pool;
    VkQueryControlFlags flags;
    uint32_t query, index;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdBeginQueryIndexedEXT);
    query_pool = checkquery_pool(L, 2, NULL);
    query = luaL_checkinteger(L, 3);
    flags = checkflags(L, 4);
    index = luaL_checkinteger(L, 5);
    ud->ddt->CmdBeginQueryIndexedEXT(cb, query_pool, query, flags, index);
    return 0;
    }

static int CmdEndQueryIndexed(lua_State *L)
    {
    ud_t *ud;
    VkQueryPool query_pool;
    uint32_t query, index;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, CmdEndQueryIndexedEXT);
    query_pool = checkquery_pool(L, 2, NULL);
    query = luaL_checkinteger(L, 3);
    index = luaL_checkinteger(L, 4);
    ud->ddt->CmdEndQueryIndexedEXT(cb, query_pool, query, index);
    return 0;
    }


static int CmdDrawIndirectByteCount(lua_State *L)
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    uint32_t instanceCount = luaL_checkinteger(L, 2);
    uint32_t firstInstance = luaL_checkinteger(L, 3);
    VkBuffer counterBuffer = checkbuffer(L, 4, NULL);
    VkDeviceSize counterBufferOffset = checkdevicesize(L, 5);
    uint32_t counterOffset = luaL_checkinteger(L, 6);
    uint32_t vertexStride = luaL_checkinteger(L, 7);
    CheckDevicePfn(L, ud, CmdDrawIndirectByteCountEXT);
    ud->ddt->CmdDrawIndirectByteCountEXT(cb, instanceCount, firstInstance,
            counterBuffer, counterBufferOffset, counterOffset, vertexStride);
    return 0;
    }

#if 0 // 10yy
        { "",  },
static int (lua_State *L) //@@ scaffolding
    {
    ud_t *ud;
    VkCommandBuffer cb = checkcommand_buffer(L, 1, &ud);
    CheckDevicePfn(L, ud, );
    ud->ddt->(cb, );
    return 0;
    }

#endif



static const struct luaL_Reg Functions[] = 
    {
        { "cmd_bind_pipeline", CmdBindPipeline },
        { "cmd_set_viewport", CmdSetViewport },
        { "cmd_set_scissor", CmdSetScissor },
        { "cmd_set_line_width", CmdSetLineWidth },
        { "cmd_set_depth_bias", CmdSetDepthBias },
        { "cmd_set_blend_constants", CmdSetBlendConstants },
        { "cmd_set_depth_bounds", CmdSetDepthBounds },
        { "cmd_set_stencil_compare_mask", CmdSetStencilCompareMask },
        { "cmd_set_stencil_write_mask", CmdSetStencilWriteMask },
        { "cmd_set_stencil_reference", CmdSetStencilReference },
        { "cmd_bind_descriptor_sets", CmdBindDescriptorSets },
        { "cmd_bind_index_buffer", CmdBindIndexBuffer },
        { "cmd_bind_vertex_buffers", CmdBindVertexBuffers },
        { "cmd_draw", CmdDraw },
        { "cmd_draw_indexed", CmdDrawIndexed },
        { "cmd_draw_indirect", CmdDrawIndirect },
        { "cmd_draw_indexed_indirect", CmdDrawIndexedIndirect },
        { "cmd_dispatch", CmdDispatch },
        { "cmd_dispatch_indirect", CmdDispatchIndirect },
        { "cmd_copy_buffer", CmdCopyBuffer },
        { "cmd_copy_image", CmdCopyImage },
        { "cmd_blit_image", CmdBlitImage },
        { "cmd_copy_buffer_to_image", CmdCopyBufferToImage },
        { "cmd_copy_image_to_buffer", CmdCopyImageToBuffer },
        { "cmd_update_buffer", CmdUpdateBuffer },
        { "cmd_fill_buffer", CmdFillBuffer },
        { "cmd_clear_color_image", CmdClearColorImage },
        { "cmd_clear_depth_stencil_image", CmdClearDepthStencilImage },
        { "cmd_clear_attachments", CmdClearAttachments },
        { "cmd_resolve_image", CmdResolveImage },
        { "cmd_set_event", CmdSetEvent },
        { "cmd_reset_event", CmdResetEvent },
        { "cmd_wait_events", CmdWaitEvents },
        { "cmd_pipeline_barrier", CmdPipelineBarrier },
        { "cmd_begin_query", CmdBeginQuery },
        { "cmd_end_query", CmdEndQuery },
        { "cmd_reset_query_pool", CmdResetQueryPool },
        { "cmd_write_timestamp", CmdWriteTimestamp },
        { "cmd_copy_query_pool_results", CmdCopyQueryPoolResults },
        { "cmd_push_constants", CmdPushConstants },
        { "cmd_begin_render_pass", CmdBeginRenderPass },
        { "cmd_next_subpass", CmdNextSubpass },
        { "cmd_end_render_pass", CmdEndRenderPass },
        { "cmd_execute_commands", CmdExecuteCommands },
        { "cmd_debug_marker_begin", CmdDebugMarkerBegin },
        { "cmd_debug_marker_end", CmdDebugMarkerEnd },
        { "cmd_debug_marker_insert", CmdDebugMarkerInsert },
        { "cmd_push_descriptor_set", CmdPushDescriptorSet },
        { "cmd_push_descriptor_set_with_template", CmdPushDescriptorSetWithTemplate },
        { "cmd_set_discard_rectangle", CmdSetDiscardRectangle },
        { "cmd_set_sample_locations", CmdSetSampleLocations },
        { "cmd_begin_debug_utils_label", CmdBeginDebugUtilsLabel },
        { "cmd_end_debug_utils_label", CmdEndDebugUtilsLabel },
        { "cmd_insert_debug_utils_label", CmdInsertDebugUtilsLabel },
        { "cmd_begin_conditional_rendering", CmdBeginConditionalRendering },
        { "cmd_end_conditional_rendering", CmdEndConditionalRendering },
        { "cmd_draw_indirect_count", CmdDrawIndirectCount },
        { "cmd_draw_indexed_indirect_count", CmdDrawIndexedIndirectCount },
        { "cmd_set_device_mask", CmdSetDeviceMask },
        { "cmd_dispatch_base", CmdDispatchBase },
        { "cmd_bind_transform_feedback_buffers", CmdBindTransformFeedbackBuffers },
        { "cmd_begin_transform_feedback", CmdBeginTransformFeedback },
        { "cmd_end_transform_feedback", CmdEndTransformFeedback },
        { "cmd_begin_query_indexed", CmdBeginQueryIndexed },
        { "cmd_end_query_indexed", CmdEndQueryIndexed },
        { "cmd_draw_indirect_byte_count", CmdDrawIndirectByteCount },
        { NULL, NULL } /* sentinel */
    };


void moonvulkan_open_cmd(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

