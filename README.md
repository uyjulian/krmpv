# mpv plugin for Kirikiri 2 / Z

This plugin allows the usage of [libmpv](https://mpv.io/) in [Kirikiri / 吉里吉里 2 / Z](https://krkrz.github.io/).  
Using libmpv allows for more codecs and more flexibility than using the `VideoOverlay` class in Kirikiri.  
This plugin does not rely on DirectShow or Windows Media Foundation, so you can display movies or videos using this plugin on Linux / SteamOS (Steam Deck) / Wine / Proton without installing additional software, or require the user to install the Media Feature Pack for Windows N on Windows when using certain editions of Windows.  
This plugin is also more predictible than using DirectShow or Windows Media Foundation, since the user may have installed codecs that can cause crashes or playback to be performed incorrectly. As such, using this plugin will reduce the amount of support requests related to movie playback.  

## Building

The build steps are described in `.github/workflows/ci.yml`.  

## How to use

After `Plugins.link("krmpv.dll");` is used, the libmpv interface will be exposed under the `KrMpv` class.  
This interface is similar to the JavaScript interface in mpv.  
Please refer to the [mpv manual](https://mpv.io/manual/master/) for more details.  

## License

This plugin is licensed under the GNU Lesser General Public License LGPL version 2.1 or later license.  

Since the GNU **Lesser** General Public License is used for this plugin, you can use this plugin without releasing the source code for the rest of your project, unlike the [Aquaplus/Leaf GPL incident](https://leaf.aquaplus.jp/product/xvid.html).  

When using this plugin in commercial releases, attribution to the author is required.  
This can be sastified by placing the plugin in the `plugins` folder and not placing the plugin in an archive that can not be opened by end-users or indexed by directory search (e.g. SteamDB identifies what libraries a game uses by its filenames).  
This allows the metadata of the plugin to be referred to by end-users.  

When making a modified version of the plugin, it must also be licensed under the GNU Lesser General Public License LGPL version 2.1 or later license.  

Please refer to the `LICENSE` file for more information about the license.  
