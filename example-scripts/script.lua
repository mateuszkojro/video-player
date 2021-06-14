local function draw_lines()
    local cf = CurrentFrame

    local smaller
    if cf.getSizeX() < cf.getSizeY() then
        smaller = cf.getSizeX()
    else
        smaller = cf.getSizeY()
    end

    for i = 0, smaller do
        cf.setPixelBGR(i, i, 0, 0, 255)
    end

    for i = 0, smaller - 20 do
        cf.setPixelBGR(i + 20, i, 0, 255, 0)
    end

    for i = 0, smaller - 40 do
        cf.setPixelBGR(i + 40, i, 255, 0, 0)
    end

-- end

-- CurrentFrame.applyHSV();

local clock_start = os.clock()
draw_lines()
local clock_stop = os.clock()
local text = "Frame time = " .. (clock_start - clock_stop)

-- addText(text, pos_x, pos_y, scale, b, g, r, thickness)
-- CurrentFrame.addText(text, 25, 25, 0.75, 0, 0, 255, 1.5)

-- CurrentFrame.applyGreyscale()
