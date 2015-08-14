class Memory
{
    static Array freelist;      // 此时freelist实际上就是个指针

    /** 初始化内存参数 */
    function void init() 
    {
        freelist = 0;
        freelist[2048] = 14334; // 14334 = 37FE(H)
        freelist[2049] = 2050;  // 2050 = 802(H)
        return;
    }
    
    /** 返回地址为address的内存单元 */
    function int peek(int address) 
    {
        return freelist[address];
    }

    /** 将整数值value存入地址为address的内存单元中设置在这个地址上的主内存的值 */
    function void poke(int address, int value) 
    {
        freelist[address] = value;
        return;
    }
    
   function int alloc(int size)
    {
        Array freeBlock;
        if (size < 1)
        {
            Sys.error(5);
        }        
        freeBlock = 2048;
        while (freeBlock[0] < size)
        {
            freeBlock = freeBlock[1];
            
            /*
            if (freeBlock == 0)
            {
                clearBlock();
                freeBlock = 2048;
                while (freeBlock[0] < size)
                {
                    freeBlock = freeBlock[1];
                    Sys.error(6);
                }
            }
            */
        }
        if (freeBlock + size > 16379)
        {
            Sys.error(6);
        }
        if (freeBlock[0] > size + 2)
        {
            freeBlock[size + 2] = freeBlock[0] - size - 2;      // 更新下一个block的length
            if (freeBlock[1] == freeBlock + 2)                  // 如果当前block是最后一个block
            {
                freeBlock[size + 3] = freeBlock + size + 4;     // 设置下一个block为最后一个block
            }
            else
            {
                freeBlock[size + 3] = freeBlock[1];             // 设置下一个block指向下下个block
            }
            freeBlock[1] = freeBlock + size + 2;
        }
        freeBlock[0] = 0;
        //initBlock(freeBlock + 2);
        return (freeBlock + 2);
    }
    
    /*
    function void initBlock(int block)
    {
        int i;
        Array arr;
        i = 0;
        while (i < arr[block - 1])
        {
            poke(block + i, 0);
            i = i + 1;
        }
    }
    */
    
    /*
    // 空闲区碎片整理
    function void clearBlock()
    {
        Array currentNodePointer;
        Array nextNodePointer;
        currentNodePointer= 2048;
        nextNodePointer = currentNodePointer[1];
        while (nextNodePointer != 0)
        {
            if (currentNodePointer != 0 & nextNodePointer != 0)
            {
                currentNodePoint[0] = currentNodePointer[1] - currentNodePointer + nextNodePointer[0];
                if (nextNodePointer[1] == nextNodePointer + 2)
                {
                    currentNodePointer[1] = currentNodePointer + 2;
                }
                else
                {
                    currentNodePointer[1] = nextNodePointer[1];
                }
            }
            currentNodePointer = nextNodePointer;
            nextNodePointer = currentNodePointer[1];
        }
    }
    */
    
    function void deAlloc(int object)
    {
        Array currentNodePointer;
        Array nextNodePointer;
        
        currentNodePointer = object - 2;
        nextNodePointer = currentNodePointer[1];
        if (nextNodePointer[0] == 0)       // 如果next不是空闲区, 直接修改length就行了
        {
            currentNodePointer[0] = currentNodePointer[1] - currentNodePointer - 2;
        }
        else                                // 如果next是空闲区, 需要合并空闲区
        {
            currentNodePointer[0] = currentNodePointer[1] - currentNodePointer + nextNodePointer[0];
            if (nextNodePointer[1] == nextNodePointer + 2)        // 如果后一个block是最后一个block, 当前block指向空
            {
                currentNodePointer[1] = currentNodePointer + 2;     
            }
            else                                                  // 否则把当前block指向后一个block指向的block
            {
               currentNodePointer[1] = nextNodePointer[1];
            }
        }        
        return;
    }
}
