local Serial = require('periphery').Serial
local serial = Serial('/dev/ttyS1', 115200)
serial:write('AT#PORTCFG=2\r')
local buf = serial:read(128, 500)
print(string.format("read %d bytes: _%s_", #buf, buf))
serial:close()

local function zeros()
    local t = {}
    for i = 1, 2048 do
        t[i] = 0
    end
    return t
end

local function cmd(s)
    local t = { #s, 0x00, 0x00, 0x00 }
    for i = 1, #s do
        t[i+4] = string.byte(s:sub(i, i))
    end
    for i = #t + 1, 2048 do
        t[i] = 0
    end
    return t
end

local SPI = require('periphery').SPI
local GPIO = require('periphery').GPIO

local mrdy = GPIO(113, 'out')
local srdy = GPIO(117, 'in')

mrdy:write(0)

local spi = SPI{
    device='/dev/spidev1.0',
    mode=1,
    max_speed=13e6,
    bit_order="msb",
    bits_per_word=8,
    extra_flags=0x04
}
print(spi)

local function transfer(tx)
    mrdy:write(1)
    while not srdy:read() do end
    local rx = spi:transfer(tx)
    mrdy:write(0)
    return rx
end

local function dump(buf)
    for i, x in ipairs(buf) do
        buf[i] = string.format('%02X', x)
    end
    for i = 1, #buf, 16 do
        print(table.concat(buf, ' ', i, i + 15))
    end
end

local function dump2ascii(buf)
    print(table.concat(buf, ' ', 1, 4))
    local len = buf[1]
    len = bit.bor(len, bit.lshift(bit.band(buf[2], 0xF), 8))
    if len > 2044 then
        return ''
    end
    local copy = {}
    for i = 5, len + 4 do
        table.insert(copy, string.char(buf[i]))
    end
    return table.concat(copy)
end

local tx = { cmd('AT+IPR?\r'), cmd('AT\r') }

print 'Sending...'
local rx = transfer(tx[1])
--print('Response', dump2ascii(rx))

print 'Sending...'
rx = transfer(tx[2])
print('Response', dump2ascii(rx))

while not srdy:read() do end
rx = transfer(zeros())
print('Response', dump2ascii(rx))

while not srdy:read() do end
rx = transfer(zeros())
print('Response', dump2ascii(rx))

while not srdy:read() do end
rx = transfer(zeros())
print('Response', dump2ascii(rx))

spi:close()
mrdy:close()
srdy:close()
