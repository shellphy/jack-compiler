class Screen 
{
    static boolean color;

    /** 初始化, 供内部使用 */
    function void init() 
    {
        color = true;
        return;
    }

    /** 清屏 */
    function void clearScreen() 
    {
        int address;
        address = 16384;
        while(address < 24576)
        {
            Memory.poke(address,0);
            address=address+1;
        }
        return;
    }

    /** 为后续的drawXXX命令设置绘图颜色(white = false, black = true) */
    function void setColor(boolean b) 
    {
        color = b;
        return;
    }

    /** 在坐标(x, y)处绘制像素 */
    function void drawPixel(int x, int y) 
    {
        int i, divNum, address, remain, temp;
        if((x > 511) | (y > 255))
        {
            Sys.error(11);
        }
        i=1;
        divNum = Math.divide(x, 16);
        address = 16384 + Math.multiply(y, 32) + divNum;
        remain = x - Math.multiply(divNum, 16);
        temp = Memory.peek(address);
        while(remain > 0)
        {
            i = i + i;
            remain = remain - 1;
        }
        //if color = false, negate the i to draw white pixel
        if(color)
        {
            temp = (temp | i);          
        }
        else
        {
            i = ~i;
            temp = (temp & i);
        }
        Memory.poke(address, temp);  
        return;
    }

    /** 在像素点(x1, y1)与像素点(x2, y2)之间画一条直线 */
    function void drawLine(int x1, int y1, int x2, int y2) 
    {
        int a, b, dx, dy, compDx, compDy, adyMinusbdx;
        
        dx = x2 - x1;
        dy = y2 - y1;
        compDx = Math.abs(dx) + 1;
        compDy = Math.abs(dy) + 1;
        a = 0;
        b = 0;
        if (dy == 0)
        {
            if(dx > 0)
            {
                while(a < compDx)
                {
                    Screen.drawPixel(x1 + a, y1);
                    a = a + 1;               
                }
                return;                
            }
            else
            {
                while(a < compDx)
                {
                    Screen.drawPixel(x1 - a, y1);
                    a = a + 1;               
                }
                return;
            }
        }
        if (dx == 0)
        {
            if(dy > 0)
            {
                while(a < compDy)
                {
                    Screen.drawPixel(x1, y1 + a);
                    a = a + 1;               
                }
                return;                
            }
            else
            {
                while(a < compDy)
                {
                    Screen.drawPixel(x1 , y1 - a);
                    a = a + 1;               
                }
                return;  
            }
        }

        if((dx > 0) & (dy > 0))
        {
            adyMinusbdx = 0;
            while((a < compDx) & (b < compDy))
            {
                Screen.drawPixel(x1 + a, y1 + b);
                if(adyMinusbdx < 0)
                {
                    adyMinusbdx = adyMinusbdx + dy;
                    a = a + 1;
                }
                else
                {
                    adyMinusbdx = adyMinusbdx - dx;
                    b = b + 1;
                }
            }
            return;
        }

        if((dx < 1) & (dy < 1))
        {
            adyMinusbdx = 0;
            while((a < compDx) & (b < compDy))
            {
                Screen.drawPixel(x2 + a, y2 + b);
                if(adyMinusbdx < 0)
                {
                    adyMinusbdx = adyMinusbdx - dy;
                    a = a + 1;
                }
                else
                {
                    adyMinusbdx = adyMinusbdx + dx;
                    b = b + 1;
                }
            }
            return;
        }

        if((dx > 0) & (dy < 1))
        {
            adyMinusbdx = 0;
            while((a < compDx) & (b < compDy))
            {
                Screen.drawPixel(x1 + a, y1 - b);
                if(adyMinusbdx < 0)
                {
                    adyMinusbdx = adyMinusbdx - dy;
                    a = a + 1;
                }
                else
                {
                    adyMinusbdx = adyMinusbdx - dx;
                    b = b + 1;
                }
            }
            return;
        }

        if((dy > 0) & (dx < 1))
        {
            adyMinusbdx = 0;
            while((a < compDx) & (b < compDy))
            {
                Screen.drawPixel(x2 + a, y2 - b);
                if(adyMinusbdx < 0)
                {
                    adyMinusbdx = adyMinusbdx + dy;
                    a = a + 1;
                }
                else
                {
                    adyMinusbdx = adyMinusbdx + dx;
                    b = b + 1;
                }
            }
            return;
        }
        return;
    }

    /** 绘制有填充色的长方形, 长方形的左上角坐标是(x1, y1), 右下角的坐标是(x2, y2) */
    function void drawRectangle(int x1, int y1, int x2, int y2) 
    {
        int y;
        
        y = y1;
        while(y < y2)
        {
            Screen.drawLine(x1, y, x2, y);
            y = y + 1;
        }
        Screen.drawLine(x1, y, x2, y);
        return;
    }

    /** 绘制圆心坐标为(x, y), 半径为r(r <= 181)的有填充色的圆 */
    function void drawCircle(int cx, int cy, int r) 
    {
        int y, dy, dyPower, rPower, halfDistance, sqrNum, xLeft, xRight;
        if((cx > 511) | (cy > 255) | (r > 181))
        {
            Sys.error(12);
        }
        dy = -r;
        rPower = Math.multiply(r, r);
        while(dy < r)
        {
            dyPower = Math.multiply(dy, dy);
            halfDistance = rPower - dyPower;
            sqrNum = Math.sqrt(halfDistance);
            xLeft = cx - sqrNum;
            xRight = cx + sqrNum;
            y = cy + dy;
            Screen.drawLine(xLeft, y, xRight, y);
            dy = dy + 1;
        }
        Screen.drawPixel(cx, cy + r);
        return;
    }
}
