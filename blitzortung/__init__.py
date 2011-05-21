#!/usr/bin/env python
# -*- coding: utf8 -*-
#-----------------------------------------------------------------------------
#   Copyright (c) 2011, Andreas Wuerl. All rights reserved.
#
#   Released under the GPLv3 license. See the LICENSE file for details.
#-----------------------------------------------------------------------------
"""
blitzortung python modules
"""
__version__ = '0.2.0'

import struct as _struct

#-----------------------------------------------------------------------------
#  Constants.
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
#   Custom exceptions.
#-----------------------------------------------------------------------------

class Error(Exception):
    """
    General Klimalogger error class.
    """
    pass


#-----------------------------------------------------------------------------
#   Public interface and exports.
#-----------------------------------------------------------------------------


from blitzortung import Config
import data
import geom
import files

__all__ = [
    'Config', # main classes
    
    'data.TimeIntervals', # data items
    
    'Db', # database access
  
    'Error', # custom exceptions

    'files.RawFiles', 'files.Data',
    'geom.Point',
]
