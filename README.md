# GlassPlotter - Desktop Glassmap Viewer

![MDI](image/Screenshot_MDI.png)

## Overview
GlassPlotter is an open source and easy-to-use glassmap viewer.  

As listed below, GlassPlotter features to support various glass maps, analytical plots and utilities. Besides, most of glass manufacturers are supported, for GlassPlotter reads glass catalogs which are freely available and are usually contained in optical design software.

It is also noteworthy that GlassPlotter is developed as a standalone application with intuitive GUI, which frees from any scripting and specific environments.  GlassPlotter aims to be useful for any diverse optical engineers, including those who are not skillful at programming.

## Functions
- Glass maps
  - [x] nd - vd
  - [x] ne - ve
  - [x] PgF - vd
  - [x] PCt - vd
  - [ ] Buchdahl coefficients (test)
- Plots
  - [x] Refractive index vs wavelength plot
  - [x] Transmittance vs wavelength plot
  - [x] Thermal coefficient (dn/dt) plot
- Table
  - [x] Various glass properties (optical, mechanical, chemical, etc)
- Others
  - [x] Glass search (used for glass fitting)

## Getting Started 
The prebuilt binaries can be downloaded from [Release page](https://github.com/heterophyllus/glassplotter/releases/latest).

See also [Wiki](https://github.com/heterophyllus/glassplotter/wiki) for further information.

## Glass Catalogs
This application reads glass catalog files in either of two formats.

1. Zemax AGF  
   Available on each manufacturer's website. 

2. CODE V XML  
   Available on [Synopsys support page](https://www.synopsys.com/optical-solutions/support/support-glass-catalog.html).

Note that the AGF/XML files in this repository are not always up to date.

## Dependency

|Library|Description|
|---|---|
|[Qt](https://www.qt.io)|GUI library|
|[QCustomPlot](https://www.qcustomplot.com)|Graph plotting library|
|[spline](https://github.com/ttk592/spline)|Cubic spline interpolation library|
|[Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) | Template library for linear algebra|
|[pugixml](https://github.com/zeux/pugixml)|Simple XML parser|


All of the aboves except Qt itself are already included in this repository.

## License
This project is licensed under GPL License - see the [LICENSE](LICENSE.md) for details.

## Contribution
Contributions and feedbacks are greatly appreciated.
Please read [CONTRIBUTING](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.
