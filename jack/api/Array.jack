class Array
{
    /** 构造大小为size的新数组 */
    function Array new(int size) 
    {
        Array a;
        a = Memory.alloc(size);
        return a;
    }
    
    /** 清除数组 */
    method void dispose() 
    {
        Memory.deAlloc(this);
        return;
    }
}