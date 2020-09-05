# GlassPlotter - Simple Glassmap Viewer

![MDI](image/Screenshot_MDI.png)

## Overview
GlassPlotter is an open source, desktop glass map viewer which is capable of visualizing Abbe diagram, plotting property curves and showing datasheets, etc. It aims to be useful for daily work of diverse optical designers.


## Key Features
- Explore glassmaps with easy-to-use GUI
  - Optical designers eager to find the best material to satisfy the target function.  This application helps them with exploring various optical glasses from different suppliers.
- Support user defined curve
  - Each of the suppliers defines its own "normal line", which is often confusing in comparing multiple glasses.  This application provides consistent displays on the same basis by plotting user defined curve which can be calculated using polynomial fitting. 

  
## Getting Started 
Executable binaries for Windows/Mac can be downloaded from [Release page](https://github.com/heterophyllus/glassplotter/releases/latest).

See also [Wiki](https://github.com/heterophyllus/glassplotter/wiki) to get how to use and other information.

## Resources
This application can extract glass data from catalog files in two formats.

1. Zemax AGF  
  AGF files are available on the respective manufacturers' web site. 

2. CODE V XML  
   CODE V XML files are available on [Synopsys support page](https://www.synopsys.com/optical-solutions/support/support-glass-catalog.html)

If Zemax or CODE V has been installed, glass catalog files are assumed to be in the folder under the installation directory.

Note that the AGF files and XML files included in this repository are not always up to date.

## Development
This application is built with:
- [Qt and QtCreator](https://www.qt.io) : GUI library and IDE
- [QCustomPlot](https://www.qcustomplot.com) : Graph plotting library
- [spline](https://github.com/ttk592/spline) : Cubic spline interpolation library
- [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) : Template library for linear algebra
- [pugixml](https://github.com/zeux/pugixml) : Simple XML parser
  
## License
This project is licensed under the GPL v3 License - see the [LICENSE](LICENSE.md) for details.

## Contribution
Contributions and feedbacks are greatly appreciated.
Please read [CONTRIBUTING](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.
