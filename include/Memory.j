class Memory 
{
    static Array freelist;      // 此时freelist实际上就是个指针

    /** 初始化内存参数 */
    function void init() 
    {
        freelist = 0;
        freelist[2048] = 14334;
        freelist[2049] = 2050;
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

    /** 从内存堆中寻找并分配一块大小为size的内存区, 返回其基地址的指针 */
    function int alloc(int size) 
    {
        int listRoom, listTag, tempAdd, returnVal;
        int minSize;
        
        minSize = size + 3;
        listTag = 2048;
        listRoom = Memory.peek(listTag);
        while(minSize > listRoom)
        {
            listTag = listTag + 1;
            listTag = Memory.peek(listTag);
            listRoom = Memory.peek(listTag);
            if(listTag == 0) 
            {
                Sys.error(7);
            }
        }
        returnVal = listTag + 2;
        Memory.poke(listTag, 0);
        listTag = listTag + 1;
        tempAdd = Memory.peek(listTag) + size;
        Memory.poke(listTag, tempAdd);
        Memory.poke(tempAdd, listRoom - size - 2);
        listTag = tempAdd + 1;
        Memory.poke(listTag, listTag + 1);          
        return returnVal;
    }

    /** 收回之前分配给对象的内存空间 */
    function void deAlloc(int object) 
    {
        int length;
        length = Memory.peek(object + 1) - object - 2;
        Memory.poke(object, length);
        return;
    }    
}
