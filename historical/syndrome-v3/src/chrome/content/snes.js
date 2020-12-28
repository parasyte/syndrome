/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Syndrome.
 *
 * The Initial Developer of the Original Code is
 * Jason Oster <parasyte at kodewerx org>.
 * Portions created by the Initial Developer are Copyright (C) 2009
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


// SNES ROM constructor
function snesROM(filepath, data) {
    this.filepath = filepath;
    this.data = data;

    this.smcheader = (this.data.length & 0x7FFF);
    this.romsize = (this.data.length - this.smcheader);
    this.romtype = this.detectMapping();
    jsdump("snesROM(): title = " + this.getString(this.getHeaderPtr(snesROM.HDR_TITLE), 21));
    jsdump("snesROM(): romtype = " + this.romtype);
}


// SNES ROM Type constants
snesROM.TYPE_UNK =          -1;
snesROM.TYPE_LOROM =        0;
snesROM.TYPE_HIROM =        1;

// SNES ROM Header constants
snesROM.HDR_TITLE =         0xFFC0; // ROM Header, Game Title           - 21 bytes
snesROM.HDR_MAKEUP =        0xFFD5; // ROM Header, ROM Makeup           - 1 byte
snesROM.HDR_ROMTYPE =       0xFFD6; // ROM Header, ROM Type             - 1 byte
snesROM.HDR_SIZE =          0xFFD7; // ROM Header, ROM Size             - 1 byte
snesROM.HDR_SRAM =          0xFFD8; // ROM Header, SRAM Size            - 1 byte
snesROM.HDR_COUNTRY =       0xFFD9; // ROM Header, Country Code         - 1 byte
snesROM.HDR_LICENSE =       0xFFDA; // ROM Header, License Code         - 1 byte
snesROM.HDR_VERSION =       0xFFDB; // ROM Header, Game Version         - 1 byte
snesROM.HDR_COMPSUM =       0xFFDC; // ROM Header, Checksum Complement  - 1 word
snesROM.HDR_CHECKSUM =      0xFFDE; // ROM Header, Checksum             - 1 word


// SNES ROM class
snesROM.prototype = {
    // Full path to the open file
    filepath: null,

    // Array containing the complete file data
    data: null,

    // SMC file header: 0 without header, 512 with header
    smcheader: 0,

    // SNES ROM size, not including SMC header
    romsize: 0,

    // SNES ROM type:
    romtype: snesROM.TYPE_UNK,


    // Convert an SNES pointer to a file offset
    ptr2offset: function(ptr) {
        var bank = 0;
        var addr = 0;

        switch (this.romtype) {
            case snesROM.TYPE_LOROM:
                bank = (ptr >> 16);
                addr = (ptr & 0x7FFF);

                if ((this.romsize > 0x00400000) && (bank <= 0x7D)) // ExLoROM
                    return (((bank << 15) | addr) + 0x00400000 + this.smcheader);
                if ((bank < 0x70) || (bank > 0x7F)) // LoROM
                    return ((((bank & 0x7F) << 15) | addr) + this.smcheader);

                throw Error("LoROM error");

            case snesROM.TYPE_HIROM:
                bank = (ptr >> 16);
                addr = (ptr & 0x003FFFFF);

                // Check for invalid HiROM address
                if (((bank < 0x78) && (!(addr & 0x8000))) || ((bank >= 0x7E) && (bank <= 0x7F))) throw Error("Invalid HiROM address");

                if (this.romsize > 0x00400000) { // ExHiROM
                    if (bank >= 0xC0) return (addr + this.smcheader);
                    return (addr + 0x00400000 + this.smcheader);
                }
                return (addr + this.smcheader);
        }

        throw Error("invalid ROM mapping type");
    },

    // Convert a file offset to an SNES pointer
    offset2ptr: function(offset) {
        var addr = (offset - this.smcheader);

        switch (this.romtype) {
            case snesROM.TYPE_LOROM:
                if ((this.romsize > 0x00400000) && (addr >= 0x00400000)) // ExLoROM
                    return ((((addr - 0x00400000) << 1) & 0x007F0000) | (addr & 0x7FFF) | 0x8000);
                return (((addr << 1) & 0x007F0000) | (addr & 0x7FFF) | 0x00808000);

            case snesROM.TYPE_HIROM:
                if ((this.romsize > 0x00400000) && (addr >= 0x00400000)) { // ExHiROM
                    if ((addr >= 0x00700000) && (addr <= 0x00780000) && (!(addr & 0x8000))) throw Error("Invalid HiROM address");
                    return addr;
                }
                return (addr | 0x00C00000);
        }

        throw Error("invalid ROM mapping type");
    },


    // Get array containing 16-bit long data
    getArray16: function(ptr, count) {
        var data = new Array(count * 2);

        if (this.romtype == snesROM.TYPE_LOROM) {
            throw Error("getArray16() does not support SNES LOROM! :(");
        }

        var addr = this.ptr2offset(ptr);
        for (var i = 0; i < count; i++) {
            data[i] = ((this.data[addr + 1] << 8) |
                       (this.data[addr + 0] << 0));
            addr += 2;
        }
        return data;
    },

    // Get array containing 8-bit long data
    getArray8: function(ptr, count) {
        var data = new Array(count);

        if (this.romtype == snesROM.TYPE_LOROM) {
            throw Error("getArray8() does not support SNES LOROM! :(");
        }

        var addr = this.ptr2offset(ptr);
        for (var i = 0; i < count; i++) {
            data[i] = this.data[addr++];
        }
        return data;
    },

    // Get 32-bit long data
    get32: function(ptr) {
        var addr = this.ptr2offset(ptr);
        return ((this.data[addr + 3] << 24) |
                (this.data[addr + 2] << 16) |
                (this.data[addr + 1] << 8) |
                (this.data[addr + 0] << 0));
    },

    // Get 24-bit pointer data
    get24: function(ptr) {
        var addr = this.ptr2offset(ptr);
        return ((this.data[addr + 2] << 16) |
                (this.data[addr + 1] << 8) |
                (this.data[addr + 0] << 0));
    },

    // Get 16-bit word data
    get16: function(ptr) {
        var addr = this.ptr2offset(ptr);
        return ((this.data[addr + 1] << 8) |
                (this.data[addr + 0] << 0));
    },

    // Get 8-bit byte data
    get8: function(ptr) {
        var addr = this.ptr2offset(ptr);
        return (this.data[addr]);
    },

    // Get ASCII string (NOT UTF-8 safe!)
    getString: function(ptr, len) {
        var str = "";
        var chr = 0;

        if (len) var iterate = function() len--;
        else var iterate = function() true;

        while (iterate()) {
            chr = this.get8(ptr);
            //if ((chr < 0x20) || (chr > 0x7E)) return str;
            if (!chr) return str;
            str += String.fromCharCode(chr);
            ptr++;
        }
        return str;
    },


    // Put 32-bit long data
    put32: function(ptr, data) {
        var addr = this.ptr2offset(ptr);
        this.data[addr + 3] = ((data >> 24) & 0xFF);
        this.data[addr + 2] = ((data >> 16) & 0xFF);
        this.data[addr + 1] = ((data >> 8) & 0xFF);
        this.data[addr + 0] = ((data >> 0) & 0xFF);
    },

    // Put 24-bit pointer data
    put24: function(ptr, data) {
        var addr = this.ptr2offset(ptr);
        this.data[addr + 2] = ((data >> 16) & 0xFF);
        this.data[addr + 1] = ((data >> 8) & 0xFF);
        this.data[addr + 0] = ((data >> 0) & 0xFF);
    },

    // Put 16-bit word data
    put16: function(ptr, data) {
        var addr = this.ptr2offset(ptr);
        this.data[addr + 1] = ((data >> 8) & 0xFF);
        this.data[addr + 0] = ((data >> 0) & 0xFF);
    },

    // Put 8-bit byte data
    put8: function(ptr, data) {
        var addr = this.ptr2offset(ptr);
        this.data[addr] = (data & 0xFF);
    },


    // Returns an SNES pointer
    getHeaderPtr: function(loc) {
        switch (this.romtype) {
            case snesROM.TYPE_LOROM: return (loc | 0x800000);
            case snesROM.TYPE_HIROM: return (loc | 0xC00000);
        }
        throw Error("invalid ROM mapping type");
    },


    // Detect SNES ROM mapping type
    detectMapping: function() {
        var type = this.romtype;
        var chk = 0;
        var cmp = 0;

        // Test for LoROM mapping
        this.romtype = snesROM.TYPE_LOROM;
        chk = this.get16(this.getHeaderPtr(snesROM.HDR_CHECKSUM));
        cmp = this.get16(this.getHeaderPtr(snesROM.HDR_COMPSUM));

        if ((chk >= 0) && (cmp >= 0) && ((chk ^ cmp) == 0xFFFF) &&
            ((this.get8(this.getHeaderPtr(snesROM.HDR_MAKEUP)) & 0x01) == snesROM.TYPE_LOROM)) {
            this.romtype = type;
            return snesROM.TYPE_LOROM;
        }

        // Test for HiROM mapping
        this.romtype = snesROM.TYPE_HIROM;
        chk = this.get16(this.getHeaderPtr(snesROM.HDR_CHECKSUM));
        cmp = this.get16(this.getHeaderPtr(snesROM.HDR_COMPSUM));

        if ((cmp >= 0) && (cmp >= 0) && ((chk ^ cmp) == 0xFFFF) &&
            ((this.get8(this.getHeaderPtr(snesROM.HDR_MAKEUP)) & 0x01) == snesROM.TYPE_HIROM)) {
            this.romtype = type;
            return snesROM.TYPE_HIROM;
        }

        this.romtype = type;
        throw Error("invalid ROM mapping type");
    }
};



// SNES Tile Map constructor
function snesTileMap(ctx, w, h, tiles, pal) {
    this.w = w;
    this.h = h;

    this.map = new Array(w * h);

    this.tiles = tiles;

    this.pal = new Array(128);
    this.decodePalette(pal);

    // Get an image buffer for pixel data
    this.img = ctx.createImageData((w * 8), (h * 8));

    this.cache = new Array();
}

// SNES Tile Map class
snesTileMap.prototype = {
    // Width and height of the tile map
    w: 0,
    h: 0,

    // SNES tile map data, and canvas ImageData object
    map: null,

    // SNES tile data, and canvas ImageData object
    tiles: null,

    // SNES palette
    pal: null,

    // Rendered image
    img: null,

    // Tile cache
    cache: null,


    // Render tile map + tiles + palette to image
    render: function(dx, dy, dw, dh, sx, sy) {
        // Default arguments
        dx = dx || 0;
        dy = dy || 0;
        dw = dw || this.w;
        dh = dh || this.h;
        sx = sx || 0;
        sy = sy || 0;


        // Scale destination args from tile values to pixel values
        dx *= 8;
        dy *= 8;
        dw *= 8;
        dh *= 8;


        // Slow path: Just render in JavaScript, for now...
        var imgdata = this.img.data;

        var tileobj = 0; // Tile object from tile map
        var offset = 0; // Tile offset from tile object
        var palidx = 0; // Palette index from tile object

        var tile = Array(8 * 8); // Palette-indexed tile data

        var old_sx = sx; // Remember the source X coordinate

        for (var y = dy; y < dh; y += 8) { // Tile height = 8 pixels
            for (var x = dx; x < dw; x += 8) { // Tile width = 8 pixels

                // Decode tile, if needed
                tileobj = this.map[sx + (sy * this.w)];
                if (typeof(this.cache[tileobj]) == "undefined") {
                    offset = (tileobj & 0x03FF) * 32; // 32 = size of SNES tile in bytes (8w * 8h = 64 pixels @ 4bpp = 32 bytes)
                    palidx = ((tileobj >> 4) & 0x01C0); // Palette = bits 10 - 12

                    var p = 0;
                    var tempdata = 0;
                    for (var i = 0; i < 16; i += 2) {
                        for (var j = 7; j >= 0; j--) {
                            tempdata  = ((this.tiles[offset + i + 0x00] >> j) & 1) << 2;
                            tempdata |= ((this.tiles[offset + i + 0x01] >> j) & 1) << 3;
                            tempdata |= ((this.tiles[offset + i + 0x10] >> j) & 1) << 4;
                            tempdata |= ((this.tiles[offset + i + 0x11] >> j) & 1) << 5;
                            tile[p++] = (tempdata | palidx);
                        }
                    }

                    // Flip the tile, if needed
                    if (tileobj & 0x4000) { // Flip horizontally
                        for (var i = 0; i < 8; i++) {
                            var src = (i * 8);
                            var dst = (i * 8) + 7;
                            for (var j = 0; j < 4; j++) {
                                [tile[dst--], tile[src++]] = [tile[src], tile[dst]];
                            }
                        }
                    }
                    if (tileobj & 0x8000) { // Flip vertically
                        for (var i = 0; i < 4; i++) {
                            var src = (i * 8);
                            var dst = ((7 - i) * 8);
                            for (var j = 0; j < 8; j++) {
                                [tile[dst++], tile[src++]] = [tile[src], tile[dst]];
                            }
                        }
                    }

                    // Decode palette-index tile data to RGB tile data
                    var p = 0;
                    var tile_rgb = Array(8 * 8 * 3); // RGB tile data
                    for (var i = 0; i < (8 * 8); i ++) {
                        tile_rgb[p++] = this.pal[tile[i] + 0]; // Red channel
                        tile_rgb[p++] = this.pal[tile[i] + 1]; // Green channel
                        tile_rgb[p++] = this.pal[tile[i] + 2]; // Blue channel
                    }

                    // Copy tile data into cache
                    this.cache[tileobj] = tile_rgb;
                }
                else {
                    var tile_rgb = this.cache[tileobj];
                }

                // Draw decoded tile to our canvas context
                var p = 0;
                for (var i = 0; i < 8; i++) {
                    var dest = ((x + ((y + i) * dw)) * 4);
                    for (var j = 0; j < 8; j++) {
                        imgdata[dest++] = tile_rgb[p++]; // Red channel
                        imgdata[dest++] = tile_rgb[p++]; // Green channel
                        imgdata[dest++] = tile_rgb[p++]; // Blue channel
                        imgdata[dest++] = 0xFF; // tile_rgb[p++]; // Alpha channel
                    }
                }
                sx++;
            }
            sx = old_sx;
            sy++;
        }

        return this.img;
    },

    // Decode SNES palette
    decodePalette: function(pal) {
        var out = 0;
        for (var i = 0; i < 128; i++) {
            this.pal[out++] = ((pal[i] << 3) & 0xF8) | ((pal[i] >>  2) & 0x07);     // Red channel
            this.pal[out++] = ((pal[i] >> 2) & 0xF8) | ((pal[i] >>  7) & 0x07);     // Green channel
            this.pal[out++] = ((pal[i] >> 7) & 0xF8) | ((pal[i] >> 13) & 0x07);     // Blue channel
            this.pal[out++] = 0xFF;                                                 // Alpha channel
        }

        // Set index 0 of all palettes to the "transparency" color
        for (var i = 64; i < 512; i += 64) {
            this.pal[i + 0] = this.pal[0];                                          // Red channel
            this.pal[i + 1] = this.pal[1];                                          // Green channel
            this.pal[i + 2] = this.pal[2];                                          // Blue channel
            this.pal[i + 3] = this.pal[3];                                          // Alpha channel
        }
    }
};
