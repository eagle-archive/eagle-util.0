
#ifndef _UTILS_INI_H
#define _UTILS_INI_H

/* the emulator supports a simple .ini file format for its configuration
 * files. See docs/ANDROID-CONFIG-FILES.TXT for details.
 */

/* an opaque structure used to model an .ini configuration file */
typedef struct IniFile   IniFile;

/* creates a new IniFile object from a config file loaded in memory.
 *  'fileName' is only used when writing a warning to stderr in case
 * of badly formed output
 */
IniFile*  iniFile_newFromMemory( const char*  text, const char*  fileName  );

/* creates a new IniFile object from a file path,
 * returns NULL if the file cannot be opened.
 */
IniFile*  iniFile_newFromFile( const char*  filePath);

/* try to write an IniFile into a given file.
 * returns 0 on success, -1 on error (see errno for error code)
 */
int       iniFile_saveToFile( IniFile*  f, const char*  filePath );

/* free an IniFile object */
void      iniFile_free( IniFile*  f );

/* returns the number of (key.value) pairs in an IniFile */
int       iniFile_getPairCount( IniFile*  f );

/* returns the value of a given key from an IniFile.
 * NULL if the key is not assigned in the corresponding configuration file
 */
const char*  iniFile_getValue( IniFile*  f, const char*  key );

/* returns a copy of the value of a given key, or NULL if defaultValue is NULL.
 * caller must free() it.
 */
char*   iniFile_getString( IniFile*  f, const char*  key, const char* defaultValue );

/* returns an integer value, or a default in case the value string is
 * missing or badly formatted
 */
int     iniFile_getInteger( IniFile*  f, const char*  key, int  defaultValue );

/* returns a 64-bit integer value, or a default in case the value string is
 * missing or badly formatted
 */
long long iniFile_getInt64( IniFile*  f, const char*  key, long long  defaultValue );

/* returns a double value, or a default in case the value string is
 * missing or badly formatted
 */
double  iniFile_getDouble( IniFile*  f, const char*  key, double  defaultValue );

/* parses a key value as a boolean. Accepted values are "1", "0", "yes", "YES",
 * "no" and "NO". Returns either 1 or 0.
 * note that the default value must be provided as a string too
 */
int     iniFile_getBoolean( IniFile*  f, const char*  key, const char*  defaultValue );

/* parses a key value as a disk size. this means it can be an integer followed
 * by a suffix that can be one of "mMkKgG" which correspond to KiB, MiB and GiB
 * multipliers.
 *
 * NOTE: we consider that 1K = 1024, not 1000.
 */
long long  iniFile_getDiskSize( IniFile*  f, const char*  key, const char*  defaultValue );

/* These functions are used to set values in an IniFile */
void iniFile_setValue( IniFile* f, const char* key, const char* value );
void iniFile_setInteger( IniFile* f, const char* key, int value );
void iniFile_setInt64( IniFile* f, const char* key, long long value );
void iniFile_setDouble( IniFile* f, const char* key, double value );
void iniFile_setBoolean( IniFile* f, const char* key, int value );
void iniFile_setDiskSize( IniFile* f, const char* key, long long size );

/* */

#endif /* _UTILS_INI_H */
