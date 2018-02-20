# XAir-Behringer Utilities
This software provides command line interface utilities for XAir Mixers from Behringer

All software in this repository is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.

This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

This repository currently provides two executables:

### *XAir_SnapBackup*
(**CURRENTLY INCOMPLETE**) - will provide an interface to save and restore individual snapshots of an XAir mixer to and from computer storage.

### *XAir_Interface*
(**Operational - In Progress**) - Provides a general interface to an XAir mixer for sending OSC commands and recieving state data.

#### Installation
Clone the repo to your local computer. Enter the top level of the repo directory and enter one of the following commands:

`make` - compile all executables into directory `bin`

`make debug` - compile all executables into directory `bin` with debugging symbols

`make release` - compile all executables into directory `bin` with optimized executables

`make XAir_Interface` - compile XAir_Interface executable into directory `bin`

`make XAir_SnapBackup` - compile XAir_Snap Backup executable into directory `bin`

`make clean` - remove all object and executable files from directorys `bin` and `obj`
