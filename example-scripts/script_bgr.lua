local function draw_lines()
    local cf = CurrentFrame

    local x,y
    local pix = 0
    local kernel = {}
    local i = 1
    local j = 1
    
    if cf.getSizeX() > 3 then 
        x = cf.getSizeX()
    else 
        x = 0
    end

    if cf.getSizeX() > 3 then 
        y = cf.getSizeY()
    else 
        y = 0
    end

    kernel[1] = {0,0,0}
    kernel[2] = {1,1,1}
    kernel[3] = {0,0,0}
    for j = 1, y-2 do
        for i = 1, x-2 do
            --b = getPixelBGR(j,i,0)
            --g = getPixelBGR(j,i,1)
            --r = getPixelBGR(j,i,2)
            --pix = round(0.229 * b + 0.587 * g + 0.114 * r)

            tym_1 = cf.getPixel(i-1,j-1) * kernel[1][1] + cf.getPixel(i,j-1) * kernel[2][1] + cf.getPixel(i+1,j-1) * kernel[3][1] 
            tym_2 = cf.getPixel(i-1,j) * kernel[1][2] + cf.getPixel(i,j) * kernel[2][2] + cf.getPixel(i+1,j) * kernel[3][2] 
            tym_3 = cf.getPixel(i-1,j+1) * kernel[1][3] + cf.getPixel(i,j+1) * kernel[2][3] + cf.getPixel(i+1,j+1) * kernel[3][3] 
            pix = tym_1 + tym_2 + tym_3
            
            b = math.floor(0.229 * pix)
            g = math.floor(0.587 * pix)
            r = math.floor(0.114 * pix)

            cf.setPixelBGR(i+1,j+1, math.floor(0.229 * pix),math.floor(0.587 * pix),math.floor(0.114 * pix))
        end
    end

end

-- CurrentFrame.applyHSV();

local clock_start = os.clock()
draw_lines()
local clock_stop = os.clock()
local text = "Frame time = " .. (clock_start - clock_stop)

-- addText(text, pos_x, pos_y, scale, b, g, r, thickness)
CurrentFrame.addText(text, 25, 25, 0.75, 0, 0, 255, 1.5)

-- CurrentFrame.applyGreyscale()