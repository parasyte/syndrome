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


/*
 * defaultTiles is an array of SNES CHR tiles;
 * one solid tile for each of the SNES palette's 16 colors
 * The stage graphics set is appended to this array after decompressing
 */
const defaultTiles = [
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
];

// Mega Man 7 Editor constructor
function mm7Editor(filepath, data, canvas) {
    this.snesROM(filepath, data);
    this.bgmap = null;
    this.fgmap = null;
    this.stage = 0;
    this.canvas = canvas;
}


// Mega Man 7 pointer constants
mm7Editor.PTR_PALDATA =         0xC08456;   // Pointers to palette data
mm7Editor.PTR_ROOMDATA =        0xC0914F;   // Pointers to room data
/*
mm7Editor.PTR_ROOMSIZE =        0xC09179;   // Pointers to room data sizes
*/
mm7Editor.PTR_FGMAPDATA =       0xC09195;   // Pointers to foreground map data
mm7Editor.PTR_BGMAPDATA =       0xC091B1;   // Pointers to background map data
mm7Editor.PTR_BLOCKDATA =       0xC091CD;   // Pointers to block data
/*
mm7Editor.PTR_TYPEDATA =        0xC091F7;   // Pointers to "block\structure type" data
*/
mm7Editor.PTR_CMPDATA =         0xC0E993;   // Pointers to compressed data
mm7Editor.PTR_STAGEHEAD =       0xC0ED2B;   // Pointers to stage headers
mm7Editor.PTR_STAGEPAL =        0xC0FB04;   // Pointers to stage palette headers
mm7Editor.PTR_OBJECTS =         0xC1D2F5;   // Pointers to object data
mm7Editor.PTR_STRUCTDATA =      0xCA5600;   // Pointers to structure data


// Mega Man 7 Editor class
mm7Editor.prototype = {
    // SNES tile map: Background
    bgmap: null,

    // SNES tile map: Foreground
    fgmap: null,

    // Mea Man 7 stage
    stage: 0,

    // Array of objects
    objects: null,

    // Destination canvas
    canvas: null,


    // Data decompression
    decompress: function(index, length) {
        jsdump("decompress(" + index.toHex() + ", " + length.toHex() + ")");
        if ((index < 0) || (index > 183)) {
            throw Error("decompress(): Index out of range: " + index);
        }

        var ptr =       (mm7Editor.PTR_CMPDATA + (index * 5));

        var source =    this.get24(ptr + 0);
        var size =      this.get16(ptr + 3);

        // Error if the size we are given does not match the size in the compressed data header.
        if (size != length) {
            throw Error("decompress(): Size error: " + length + ", " + size);
        }

        var output =    new Array(size);
        var out =       0;
        while (out < size) {
            var ctrl = this.get8(source++);

            for (var i = 0; i < 8; i++) {
                if (ctrl & 0x80) {
                    var len = (this.get8(source) >> 2); // Length of data to copy from moving window
                    var win = (((this.get8(source) & 3) << 8) | this.get8(source + 1)); // LZ window source
                    source += 2;
                    while (len--) {
                        output[out] = output[out - win];
                        if (++out >= size) return output;
                    }
                }
                else {
                    output[out++] = this.get8(source++);
                }
                ctrl <<= 1;
            }
        }

        return output;
    },

    // Load a palette index
    loadPal: function(pal, index) {
        var ptr = (0x00C00000 | this.get16(mm7Editor.PTR_PALDATA + (index * 2)));
        /* Note: ptr now points to palette headers
         *  byte-0: length
         *  byte-1 & byte-2: pointer to palette data
         *  byte-3: destination for this palette
         */

        var len = 0;
        while ((len = this.get8(ptr++))) {
            var src = (0x00C40000 | this.get16(ptr));
            ptr += 2;

            var dst = this.get8(ptr++);
            pal.merge(dst, this.getArray16(src, len));
        }

        return pal;
    },

    // Load stage palette with sub-palette (used for swapping the palette mid-stage)
    loadStagePal: function(max) {
        if ((this.stage < 0) || (this.stage > 13)) throw Error("this.stage out of range! " + this.stage);

        var palette = new Array(4);
        for (var subpal = 0; subpal < max; subpal++) {
            var temppal = new Array(128);
            var ptr = (mm7Editor.PTR_STAGEPAL + this.get16(mm7Editor.PTR_STAGEPAL + (this.stage * 2)) + (subpal * 3));
            /* Note: ptr now points to the sub-palette header
             *  byte-0: palette index
             *  byte-1 & byte-2: background color (transparency)
             */
            // Note that the sub-palettes all inherit from one another;
            // The game just overwrites the colors that need to be changed, and leaves the rest alone.
            temppal = this.loadPal(temppal, this.get8(ptr++));
            temppal[0] = this.get16(ptr);

            palette[subpal] = temppal;
        }

        return palette;
    },

    // Load a tile set index
    loadTileSet: function(index) {
        var ptr = (0x00C00000 | this.get16(mm7Editor.PTR_STAGEHEAD + ((this.stage + 0x10) * 2)));
        /* Note: ptr now points to the stage graphics header:
         *  byte-0: Compression index
         *  byte-1 & byte-2: Decompressed data size
         *  byte-3: unknown
         *  byte-4: unknown
         *  byte-5: unknown
         */
        var cmp_index   = this.get8 (ptr + 0);
        var cmp_length  = this.get16(ptr + 1);
        var cmp_dest    = this.get16(ptr + 3);

        ptr += 3;
        jsdump("loadTileSet(): unknown: " + ptr.toHex() + "[" + this.get8(ptr + 0) + ", " + this.get8(ptr + 1) + ", " + this.get8(ptr + 2) + "]");

        // FIXME: Verify this is the correct way to handle this case ... works for Spring Man"s stage
        var tiles = defaultTiles.slice(0);
        if (cmp_dest == 0x1100) {
            cmp_dest = 0x0200;
        }
        else {
            jsdump("buildStage(): Adjusting tile size to " + cmp_dest.toHex() + " bytes");
            for (var i = 0x0200; i < cmp_dest; i++) {
                tiles[i] = 0;
            }
        }
        tiles.merge(cmp_dest, this.decompress(cmp_index, cmp_length));

        return tiles;
    },

    // Load objects
    loadObjects: function() {
        var ptr = (0x00C10000 | this.get16(mm7Editor.PTR_OBJECTS + (this.stage * 2)));
        var data = [ 0, 0, 0, 0, 0, 0, 0 ];
        this.objects = new Array();

        while (!((data[0] = this.get8(ptr++)) & 0x80)) {
            data.merge(1, this.getArray8(ptr, 6));
            ptr += 6;
            this.objects.push(new mm7Object(data));
        }
    },

    buildObjects: function() {
        this.loadObjects();
        jsdump("buildObjects (" + editor.objects.length + "): " + editor.objects.toSource());

        var ctx = this.canvas.getContext("2d");
        ctx.fillStyle = "#FF007F"; // pink

        for (var i = 0; i < this.objects.length; i++) {
            var x = this.objects[i].x;
            var y = this.objects[i].y;

            // Draw object index
            ctx.fillText(i, x + 2, y - 1);

            // Push context
            ctx.save();
            ctx.fillStyle = "black";

            // Cross-hair, horizontal
            ctx.fillRect(x - 5, y, 11, 1);

            // Cross-hair, vertical
            ctx.fillRect(x, y - 5, 1, 11);

            // Pull context
            ctx.restore();
        }
    },

    getStage: function() {
        return this.stage;
    },

    nextStage: function() {
        if (++this.stage > 13) this.stage = 0;
        return this.stage;
    },

    prevStage: function() {
        if (--this.stage < 0) this.stage = 13;
        return this.stage;
    },

    // Build stage images, tile maps, palettes, etc.
    buildStage: function() {
        if (arguments.length >= 1) {
            var stage = arguments[0];
            if ((stage < 0) || (stage > 13)) {
                throw Error("buildstage(): Stage error: " + stage);
            }
            this.stage = stage;
        }
        jsdump("buildStage(" + this.stage + ")");


        // Get foreground width and height
        var ptr = (0x00C10000 | this.get16(mm7Editor.PTR_FGMAPDATA + (this.stage * 2)));
        /* Note: ptr now points to the stage room data
         *  byte-0: stage width (in rooms)
         *  byte-1: stage height (in rooms)
         *  byte-2 to byte-n: room numbers (where n = (width * height) + 1)
         */
        var w = this.get8(ptr++)
        if ((w < 0) || (w > 32)) throw Error("buildStage(): stage width boundary: " + w);
        var h = this.get8(ptr++)
        if ((h < 0) || (h > 32)) throw Error("buildStage(): stage height boundary: " + h);


        // Compile the fgmap
        var len = (w * h);
        var rooms = this.getArray8(ptr++, len);
        var room_list = new Array();
        for (var i = 0; i < len; i++) {
            if (!room_list.some(function(el) el == rooms[i])) {
                room_list.push(rooms[i]);
            }
        }
        jsdump("rooms = " + rooms.toSource());
        jsdump("room_list = " + room_list.sort(function(a, b) a > b).toSource());


        // Load tilesets and palettes
        var tileset = new Array(this.loadTileSet(0));//, this.loadTileSet(1), this.loadTileSet(2));
        var palette = this.loadStagePal(4); // Load all four palettes


        // Resize canvas element
        var canvas_w = (w * 256);
        var canvas_h = (h * 256);
        jsdump("canvas: " + canvas_w + " x " + canvas_h + " px");
        this.canvas.setAttribute("width",  canvas_w);
        this.canvas.setAttribute("height", canvas_h);


        // Create SNES tile maps
        var ctx = this.canvas.getContext("2d");
        var len = room_list.length;
        this.fgmap = new Array(len);
        for (var i = 0; i < len; i++) {
            this.fgmap[room_list[i]] = new snesTileMap(ctx, 32, 32, tileset[0], palette[0]); // FIXME: dynamically choose tileset and palette here
            this.buildRoom(this.fgmap[room_list[i]], room_list[i]);
            this.fgmap[room_list[i]].render();
        }


        // Render only foreground
        var len = rooms.length;
        var i = 0;
        for (var y = 0; y < canvas_h; y += 256) {
            for (var x = 0; x < canvas_w; x += 256) {
                ctx.putImageData(this.fgmap[rooms[i++]].img, x, y, 0, 0, 256, 256);
            }
        }
    },

    // Build a single room (made of 8x8 structures)
    buildRoom: function(map, num) {
        var ptr = this.get24(mm7Editor.PTR_ROOMDATA + (this.stage * 3)) + (num * 128); // 128: each room is 8*8 blocks (each block is 2 bytes)

        var data = this.getArray16(ptr, 8 * 8);
        var i = 0;
        for (var y = 0; y < 32; y += 4) {
            for (var x = 0; x < 32; x += 4) {
                this.buildStruct(map, x, y, data[i++]);
            }
        }
    },

    // Build a single structure (made of 2x2 blocks)
    buildStruct: function(map, x, y, num) {
        var ptr = (mm7Editor.PTR_STRUCTDATA + this.get24(mm7Editor.PTR_STRUCTDATA + (this.stage * 3)) + num); // The structure pointers are relative
        var tx = (x + 4);
        var ty = (y + 4);
        var old_x = x;

        while (y < ty) {
            x = old_x;
            while (x < tx) {
                this.buildBlock(map, x, y, this.get16(ptr));
                ptr += 2;
                x += 2;
            }
            y += 2;
        }
    },

    // Build a single block (made of 2x2 tiles)
    buildBlock: function(map, x, y, num) {
        var ptr = this.get24(mm7Editor.PTR_BLOCKDATA + (this.stage * 3)) + (num * 8); // 8: each block is 2*2 tiles (each tile is 2 bytes)
        var tx = (x + 2);
        var ty = (y + 2);
        var old_x = x;

        while (y < ty) {
            x = old_x;
            while (x < tx) {
                map.map[x + (y * map.w)] = this.get16(ptr);
                ptr += 2;
                x++;
            }
            y++;
        }
    }

};

// Voodoo to inherit from the snesROM object
mm7Editor.inherit(snesROM);


// Mega Man 7 Objects
function mm7Object(data) {
    this.unk = data.slice(0, 3);
    this.x = (data[4] << 8) | data[3];
    this.y = (data[6] << 8) | data[5];
}

mm7Object.prototype = {
    // Unknown: 3-byte array containing information about object type, etc.
    unk: null,

    // Object coordinates
    x: 0,
    y: 0
};
