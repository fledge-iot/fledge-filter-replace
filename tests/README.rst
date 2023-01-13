=====================================================
Fledge "Replace" C++ Filter plugin unit test cases
====================================================

Replace characters in the names of assets and data points.

Configuration
-------------

replace
    The set of characters that should be replaced. Each occurrence of the character in the name of the asset or datapoint will be replaced by the replacement character

replacement
    The character to use to replace the specified set of characters. Only a single character can be supplied, any additional characters will be ignored.

Build
-----
To build Fledge "replace" C++ filter plugin unit test cases:

.. code-block:: console

  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ ./RunTests
