int2ssl
=======

#### 8.4.9
- Fixed incorrect mnemonic names in script dumps

#### 8.4.8
- Fixed `break` statement decompilation

#### 8.4.7
- Fixed unable to decompile some old scripts due to incorrect detection of the ternary operator
- Fixed treating imported procedures as procedures without a body

#### 8.4.6
- Added support for additional universal opcodes `sfall_func7` and `sfall_func8`

#### 8.4.5
- Added new logical operators `AndAlso`, `OrElse`
- Added support for new `div` operator (unsigned integer division)

#### 8.4.4
- Added support for new opcodes `register_hook_proc_spec`, `reg_anim_callback`

#### 8.4.3 (2018-01-03)
- AppVeyor configuration (alexeevdv)
- Enabled code for `ceil` math function
- Fixed missing argument for `how_much` function

#### 8.4.2 (2018-01-03)
- Fixed some errors when parsing exported variable and procedure sections (phobos2077)
- Replace linebreaks with escape sequence when storing string constants (phobos2077)
- Added support for new universal sfall opcodes (phobos2077)

#### 8.3.1 (2018-01-03)
- Files for debian packaging (alexeevdv)
- TravisCI configuration (alexeevdv)

#### 8.3.0
