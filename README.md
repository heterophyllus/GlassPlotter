# GlassPlotter - Simple Glassmap Viewer

![MDI](image/Screenshot_MDI.png)

## Overview
GlassPlotter is an open source, comprehensive glass map viewer. It aims to be useful for daily work of diverse optical designers.


## Key Features
- Explore glassmaps with easy-to-use GUI
  - Optical designers eager to find the best material to satisfy the target function.  This application provides the supportive environment to explore various optical glasses from different suppliers.
- Support user defined curve
  - Optical designers are often confused by the different definition of "normal line".  This application enables the consistent display on the single norm defined by the user.

  
## Getting Started 
Executable binaries for Windows/Mac can be downloaded from [Release page](https://github.com/heterophyllus/glassplotter/releases/latest).

See also [Wiki](https://github.com/heterophyllus/glassplotter/wiki) to get more information.

## Resources
This application utilizes glass data from catalog files in two formats.

1. Zemax AGF  
  AGF files are available on every manufacturers' web site. 

2. CODE V XML  
   CODE V XML files are available on [Synopsys support page](https://www.synopsys.com/optical-solutions/support/support-glass-catalog.html)

Note that the AGF/XML files included in this repository are not always up to date.

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
