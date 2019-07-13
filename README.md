# mpv plugin for Kirikiri

This plugin allows the usage of libmpv in Kirikiri / 吉里吉里

## Building

After cloning submodules and placing `ncbind` and `tp_stub` in the parent directory, a simple `make` will generate `krmpv.dll`.

## How to use

After `Plugins.link("krmpv.dll");` is used when `mpv-1.dll` can be located, the libmpv interface will be exposed under the `krmpv` class.

## License

This project is licensed under the GNU Lesser General Public License LGPL version 2 or
later license. Please read the `LICENSE` file for more information.
