# mpv plugin for Kirikiri 2 / Z

This plugin allows the usage of [libmpv](https://mpv.io/) in [Kirikiri / 吉里吉里 2 / Z](https://krkrz.github.io/).  
Using libmpv allows for more codecs and more flexibility than using the `VideoOverlay` class in Kirikiri.  

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
