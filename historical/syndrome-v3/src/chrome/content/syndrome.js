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


// Simple utility methods
Number.prototype.toDec = function() {
    return parseInt(this, 16);
}

Number.prototype.toHex = function() {
    return "0x" + this.toString(16).toUpperCase();
}


Array.prototype.merge = function(index, source) {
    var me = this;
    source.forEach(function(el, i, self) {
        me[index + i] = el;
    });
}


Function.prototype.inherit = function(parent) {
    if (this == parent) {
        return;
    }
    for (var prop in parent.prototype) {
        if ((typeof parent.prototype[prop] == "function") && (!this.prototype[prop])) {
            this.prototype[prop] = parent.prototype[prop];
        }
    }
    this.prototype[parent.getName()] = parent;
}

Function.prototype.getName = function() {
    var str;
    str = this.toString();
    str = str.substring(str.indexOf(" ") + 1, str.indexOf("("));
    if (str.charAt(0) == "(") {
        str = "error";
    }
    return str;
}


var editor = null;

function OpenSMC(file) {
    var istream = Components.classes["@mozilla.org/network/file-input-stream;1"]
        .createInstance(Components.interfaces.nsIFileInputStream);
    istream.init(file, -1, -1, false);

    var bstream = Components.classes["@mozilla.org/binaryinputstream;1"]
        .createInstance(Components.interfaces.nsIBinaryInputStream);
    bstream.setInputStream(istream);

    var data = bstream.readByteArray(bstream.available());

    bstream.close();
    istream.close();

    return data;
}

function OpenZIP(file) {
    var zreader = Components.classes["@mozilla.org/libjar/zip-reader;1"]
        .createInstance(Components.interfaces.nsIZipReader);
    zreader.open(file);

    // Read just the first entry
    var smcfiles = zreader.findEntries("*.smc");
    if (!smcfiles.hasMore()) return null;
    var istream = zreader.getInputStream(smcfiles.getNext());

    var bstream = Components.classes["@mozilla.org/binaryinputstream;1"]
        .createInstance(Components.interfaces.nsIBinaryInputStream);
    bstream.setInputStream(istream);

    var data = bstream.readByteArray(bstream.available());

    bstream.close();
    istream.close();
    zreader.close();

    return data;
}

function openFile() {
    const nsIFilePicker = Components.interfaces.nsIFilePicker;

    var fp = Components.classes["@mozilla.org/filepicker;1"]
        .createInstance(nsIFilePicker);

    fp.init(window, null, nsIFilePicker.modeOpen);
    fp.appendFilter("SNES ROM Files", "*.smc; *.zip"); // FIXME: L10n
    fp.appendFilters(nsIFilePicker.filterAll);

    if (fp.show() == nsIFilePicker.returnOK) {
        var file = fp.file;
        var path = file.path;
        var name = file.leafName;

        if (name.substr(-4) == ".smc") {
            var data = OpenSMC(file);
        }
        else if (name.substr(-4) == ".zip") {
            var data = OpenZIP(file);
        }
        else {
            jsdump("Unknown file type: " + name);
            return;
        }

        // Hopefully this will catch any file read errors?
        if ((data == undefined) || (data == null)) {
            jsdump("Error reading file: " + name);
            return;
        }

        var canvas = document.getElementById("syndrome_canvas");
        editor = new mm7Editor(path, data, canvas);
        refresh();
    }
}

function saveCanvas(canvas, destFile) {
    // convert string filepath to an nsIFile
    var file = Components.classes["@mozilla.org/file/local;1"]
        .createInstance(Components.interfaces.nsILocalFile);
    file.initWithPath(destFile);

    // create a data url from the canvas and then create URIs of the source and targets
    var io = Components.classes["@mozilla.org/network/io-service;1"]
        .getService(Components.interfaces.nsIIOService);
    var source = io.newURI(canvas.toDataURL("image/png", ""), "UTF8", null);

    // prepare to save the canvas data
    var persist = Components.classes["@mozilla.org/embedding/browser/nsWebBrowserPersist;1"]
        .createInstance(Components.interfaces.nsIWebBrowserPersist);

    persist.persistFlags = Components.interfaces.nsIWebBrowserPersist.PERSIST_FLAGS_REPLACE_EXISTING_FILES;
    persist.persistFlags |= Components.interfaces.nsIWebBrowserPersist.PERSIST_FLAGS_AUTODETECT_APPLY_CONVERSION;

    // save the canvas data to the file
    persist.saveURI(source, null, null, null, null, file);
}

function exportPNG() {
    if (editor) {
        const nsIFilePicker = Components.interfaces.nsIFilePicker;

        var fp = Components.classes["@mozilla.org/filepicker;1"]
            .createInstance(nsIFilePicker);

        fp.init(window, null, nsIFilePicker.modeSave);
        fp.appendFilter("PNG Image Files", "*.png"); // FIXME: L10n

        var fpShow = fp.show();
        if ((fpShow == nsIFilePicker.returnOK) || (fpShow == nsIFilePicker.returnReplace)) {
            var file = fp.file;
            var path = file.path;
            var name = file.leafName;

            // Add extension if one isn"t given
            if (name.slice(-4) != ".png") path += ".png";

            saveCanvas(editor.canvas, path);
        }
    }
}

function refresh() {
    if (editor) {
        var now = new Date();
        var time_start = now.getTime();
        delete now;


        try {
/*
            // DEBUGGY!
            connectShark();
            startShark();
*/
            editor.buildStage();
/*
            // DEBUGGY!
            stopShark();
            disconnectShark();
*/

            // Object data handling
            editor.buildObjects();
        }
        catch(e) {
            var path = editor.filepath.split("/");
            if (path.length <= 1) path = editor.filepath.split("\\");
            var name = path.pop();

            alert(name + " could not be loaded.\n(Exception while processing SNES ROM data)\n\nName: " + e.name + "\nMessage: " + e.message); // FIXME: L10n
            delete editor;
            return;
        }

        var now = new Date();
        var time_end = now.getTime();
        delete now;

        jsdump("Stage drawn in " + ((time_end - time_start) / 1000) + " seconds");
    }
}

function quit(force) {
    force = force || false;
    var appStartup = Components.classes['@mozilla.org/toolkit/app-startup;1'].
                        getService(Components.interfaces.nsIAppStartup);

    var quitSeverity = force ? Components.interfaces.nsIAppStartup.eForceQuit :
                               Components.interfaces.nsIAppStartup.eAttemptQuit;
    appStartup.quit(quitSeverity);
}

//window.addEventListener("resize", refresh, true);
