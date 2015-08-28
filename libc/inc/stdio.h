/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *  Ghost, a micro-kernel based operating system for the x86 architecture    *
 *  Copyright (C) 2015, Max Schlüssel <lokoxe@gmail.com>                     *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __GHOST_LIBC_STDIO__
#define __GHOST_LIBC_STDIO__

#include "ghost.h"
#include "sys/types.h"
#include <stddef.h>
#include <stdarg.h>

// (N1548-7.21.1-2)
#include "file.h"

__BEGIN_C

// (N1548-7.21.1-2)
typedef off_t fpos_t;

// for use with <setvbuf> (N1548-7.21.1-3)
#define _IOFBF			1 // full buffering
#define _IOLBF			2 // line buffering
#define _IONBF			3 // no buffering

// buffer sizes for <setbuf> (N1548-7.21.1-3)
#define BUFSIZ			0x2000
#define BUFSIZMIN		128

// end-of-file (N1548-7.21.1-3)
#define EOF				-1

// maximum number of files open (N1548-7.21.1-3)
#define FOPEN_MAX		1024

// maximum length of file names (N1548-7.21.1-3)
#define FILENAME_MAX	G_FILENAME_MAX

// minimum length of a temporary file name string generated by <tmpnam> (N1548-7.21.1-3)
#define L_tmpnam		G_PATH_MAX

// seek constants for use with <fseek> (N1548-7.21.1-3)
#define SEEK_CUR		1
#define SEEK_END		2
#define SEEK_SET		3

// maximum number of temporary files (N1548-7.21.1-3)
#define TMP_MAX			FOPEN_MAX

// stdin, stdout, stderr (N1548-7.21.1-3)
extern FILE* stderr;
#define stderr			stderr
extern FILE* stdin;
#define stdin			stdin
extern FILE* stdout;
#define stdout			stdout

// (posix)
#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2

/**
 * Removes the given file. (N1548-7.21.4.1)
 *
 * @param filename
 * 		name of the file to remove
 * @return
 * 		zero if successful, otherwise non-zero
 */
int remove(const char *filename);
int unlink(const char *filename);

/**
 * Renames the file <old_name> to <new_name>. (N1548-7.21.4.2)
 *
 * @param old_name
 * 		name of the file to rename
 * @param new_name
 * 		new name
 * @return
 * 		zero if successful, otherwise non-zero
 */
int rename(const char *old_name, const char *new_name);

/**
 * Creates a temporary binary file. (N1548-7.21.4.3)
 *
 * @return
 * 		pointer to the temporary file structure
 */
FILE* tmpfile();

/**
 * Creates the name of a temporary file. (N1548-7.21.4.4)
 *
 * @param buf
 * 		pointer to a buffer of at least <L_tmpnam> bytes or
 * 		alternatively a NULL pointer. if NULL is given, the
 * 		result is stored in the internal buffer
 * @return
 * 		if not successful, NULL is returned;
 * 		if <buf> was NULL, the internal buffer is returned;
 * 		otherwise the value of <buf> is returned
 */
char* tmpnam(char* buf);

/**
 * Closes the given stream. (N1548-7.21.5.1)
 *
 * @param stream
 * 		file to close
 * @return
 * 		zero if successful, otherwise <EOF>
 */
int fclose(FILE* stream);

/**
 * Flushes the given <stream>. (N1548-7.21.5.2)
 *
 * @param stream
 * 		file to flush
 * @return
 * 		zero if successful, otherwise <EOF>
 */
int fflush(FILE* stream);

/**
 * Opens the file <filename> with the given <mode>. (N1548-7.21.5.3)
 *
 * @param filename
 * 		name of the file to open
 * @param mode
 * 		mode flags
 * @return
 * 		pointer to the file structure if successful,
 * 		otherwise a NULL pointer
 */
FILE* fopen(const char* filename, const char* mode);

/**
 * Closes the file <stream> and reuses the file structure
 * to open <filename> with <mode> arguments. (N1548-7.21.5.4)
 *
 * @param filename
 * 		name of the file to open
 * @param mode
 * 		mode flags
 * @return
 * 		pointer to <stream> if successful,
 * 		otherwise a NULL pointer
 */
FILE* freopen(const char* filename, const char* mode, FILE* stream);

/**
 * See <setvbuf>. (N1548-7.21.5.5)
 */
void setbuf(FILE* stream, char* buf);

/**
 * Sets the buffering mode of <stream> to <mode>.
 * If a <buf> is given, <size> indicates the size of it and the
 * buffer might be used as the streams buffer.
 * If <buf> is NULL, <size> may be used as the size of the stream
 * buffer to allocate. (N1548-7.21.5.6)
 *
 * @param stream
 *		file to set buffering mode for
 * @param buf
 * 		if not a NULL pointer, the given buffer might be used
 * 		as the stream buffer
 * @param mode
 * 		mode to set
 * @param size
 * 		if buf is not NULL, the size of the buffer;
 * 		otherwise used as an indicator for the size of the
 * 		buffer that is allocated
 * @return
 * 		zero on success, otherwise non-zero
 */
int setvbuf(FILE* stream, char* buf, int mode, size_t size);

/**
 * Formatted printing to <stream>. (N1548-7.21.6.1)
 *
 * @param stream
 * 		stream to print to
 * @param format
 * 		format string to use
 * @varargs
 * 		optional list of arguments
 * @return
 * 		number of characters transmitted, or a negative value
 *		if an output or encoding error occurred
 */
int fprintf(FILE* stream, const char* format, ...);

/**
 * Formatted reading from <stream>. (N1548-7.21.6.2)
 *
 * @param stream
 * 		stream to read from
 * @param format
 * 		format string to use
 * @varargs
 * 		optional list of arguments
 * @return
 * 		<EOF> if error before first conversion, otherwise
 * 		number of successful conversions
 */
int fscanf(FILE* stream, const char* format, ...);

/**
 * Formatted printing to <stdout>. (N1548-7.21.6.3)
 *
 * @param format
 * 		format string to use
 * @varargs
 * 		optional list of arguments
 * @return
 *		see <fprintf>
 */
int printf(const char* format, ...);

/**
 * Formatted printing using the given callback implementation for output.
 *
 * @param param
 * 		is passed to the callback
 * @param callback
 * 		callback to use
 * @param format
 * 		format string to use
 * @varargs
 * 		optional list of arguments
 */
int vcbprintf(void* param,
		ssize_t (*callback)(void* param, const char* buf, size_t maximum),
		const char *format, va_list arglist);
int cbprintf(void* param,
		ssize_t (*callback)(void* param, const char* buf, size_t maximum),
		const char *format, ...);

/**
 * Formatted reading from <stdin>. (N1548-7.21.6.4)
 *
 * @param format
 * 		format string to use
 * @varags
 * 		optional list of arguments
 * @return
 *		see <fscanf>
 */
int scanf(const char* format, ...);

/**
 * Formatted printing into the array <s>. Prints at
 * maximum <n> characters. (N1548-7.21.6.5)
 *
 * @param s
 * 		array to print to
 * @param n
 * 		maximum number of characters - 1
 * @param format
 * 		format to use
 * @varargs
 * 		optional list of arguments
 * @return
 * 		if there was an encoding error -1;
 * 		if the output was truncated -1;
 * 		otherwise number of characters stored (excluding \0)
 */
int snprintf(char* s, size_t n, const char* format, ...);

/**
 * Formatted printing into the array <s>. (N1548-7.21.6.6)
 *
 * @param s
 * 		array to print to
 * @param format
 * 		format to use
 * @varargs
 * 		optional list of arguments
 * @return
 * 		if there was an encoding error -1;
 * 		otherwise number of characters stored (excluding \0)
 */
int sprintf(char* s, const char* format, ...);

/**
 * Formatted reading from the string <s>. (N1548-7.21.6.7)
 *
 * @param s
 * 		string to read from
 * @param format
 * 		format to use
 * @varags
 * 		optional list of arguments
 * @return
 * 		see <fscanf>
 */
int sscanf(const char* s, const char* format, ...);

/**
 * Variable arguments equivalent for <fprintf>. (N1548-7.21.6.8)
 */
int vfprintf(FILE* stream, const char* format, va_list arg);

/**
 * Variable arguments equivalent for <fscanf>. (N1548-7.21.6.9)
 */
int vfscanf(FILE* stream, const char* format, va_list arg);

/**
 * Variable arguments equivalent for <printf>. (N1548-7.21.6.10)
 */
int vprintf(const char* format, va_list arg);

/**
 * Variable arguments equivalent for <scanf>. (N1548-7.21.6.11)
 */
int vscanf(const char* format, va_list arg);

/**
 * Variable arguments equivalent for <snprintf>. (N1548-7.21.6.12)
 */
int vsnprintf(char* s, size_t n, const char* format, va_list arg);

/**
 * Variable arguments equivalent for <sprintf>. (N1548-7.21.6.13)
 */
int vsprintf(char* s, const char* format, va_list arg);

/**
 * Variable arguments equivalent for <sscanf>. (N1548-7.21.6.14)
 */
int vsscanf(const char* s, const char* format, va_list arg);

/**
 * Reads the next byte from <stream>. (N1548-7.21.7.1)
 *
 * @param stream
 * 		stream to read from
 * @return
 * 		the read unsigned char converted to an integer if successful,
 * 		otherwise <EOF>
 */
int fgetc(FILE* stream);

/**
 * Reads a string from <stream>, with at most n - 1 bytes or to the first
 * new-line character or <EOF>, and writes it to the buffer <s> with null-termination.
 * (N1548-7.21.7.2)
 *
 * @param s
 * 		buffer to read into
 * @param n
 * 		maximum number of bytes
 * @param stream
 * 		stream to read from
 * @return
 * 		<s> if successful, otherwise a <NULL> pointer
 */
char* fgets(char* s, int n, FILE* stream);

/**
 * Writes the given byte to <stream>. (N1548-7.21.7.3)
 *
 * @param c
 * 		byte, written as an unsigned char
 * @param stream
 * 		stream to write to
 * @return
 * 		the written char if successful, otherwise <EOF>
 */
int fputc(int c, FILE* stream);

/**
 * Writes the string <s> to <stream>, not writing the
 * null-terminator. (N1548-7.21.7.4)
 *
 * @param s
 * 		string to write
 * @param stream
 * 		stream to write to
 * @return
 * 		a non-negative value if successful, otherwise <EOF>
 */
int fputs(const char* s, FILE* stream);

/**
 * Equivalent to <fgetc>. (N1548-7.21.7.5)
 */
int getc(FILE* stream);

/**
 * Equivalent to <ungetc>.
 */
int fungetc(int c, FILE* stream);

/**
 * Equivalent to <getc> with <stdin> as the stream. (N1548-7.21.7.6)
 */
int getchar();

/**
 * Equivalent to <fputc>. (N1548-7.21.7.7)
 */
int putc(int c, FILE* stream);

/**
 * Equivalent to <putc> with <stdout> as the stream. (N1548-7.21.7.8)
 */
int putchar(int c);

/**
 * Writes the string <s> to <stdout>, not writing the
 * null-terminator, appending a new-line character. (N1548-7.21.7.9)
 *
 * @param s
 * 		string to write
 * @param stream
 * 		stream to write to
 * @return
 * 		a non-negative value if successful, otherwise <EOF>
 */
int puts(const char *s);

/**
 * Pushes the given character <c> back to the streams read buffer. (N1548-7.21.7.10)
 *
 * @param c
 * 		character to put
 * @param stream
 * 		stream to put the character back on
 * @return
 * 		pushed character after conversion if successful, otherwise <EOF>
 */
int ungetc(int c, FILE* stream);

/**
 * Reads up to <nmemb> with a size of <size> each from the <stream>
 * to the buffer <ptr>. (N1548-7.21.8.1)
 *
 * @param ptr
 * 		target buffer
 * @param size
 * 		size in bytes of each element
 * @param nmemb
 * 		number of elements to read
 * @param stream
 *		source stream
 * @return
 * 		number of elements read if successful, otherwise zero
 */
size_t fread(const void* ptr, size_t size, size_t nmemb, FILE* stream);

/**
 * Writes up to <nmemb> with a size of <size> each from the buffer <ptr>
 * to the <stream>. (N1548-7.21.8.2)
 *
 * @param ptr
 * 		source buffer
 * @param size
 * 		size in bytes of each element
 * @param nmemb
 * 		number of elements to write
 * @param stream
 * 		target stream
 * @return
 * 		number of elements written if successful, otherwise zero
 */
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

/**
 * Stores the current stream position in the <pos> buffer. (N1548-7.21.9.1)
 *
 * @param stream
 * 		stream to get the position from
 * @param pos
 * 		target position buffer
 * @return
 * 		zero if successful, otherwise non-zero
 */
int fgetpos(FILE* stream, fpos_t* pos);

/**
 * Repositions the file position indicator. (N1548-7.21.9.2)
 *
 * @param stream
 * 		stream to reposition
 * @param offset
 * 		offset relative to the <whence>
 * @param whence
 * 		<SEEK_CUR>, <SEEK_END> or <SEEK_SET>
 * @return
 * 		zero if successful, otherwise non-zero
 */
int fseek(FILE* stream, long int offset, int whence);
int fseeko(FILE* stream, off_t offset, int whence);

/**
 * Restores the current stream position from the <pos> buffer. (N1548-7.21.9.3)
 *
 * @param stream
 * 		stream to set the position from
 * @param pos
 * 		source position buffer
 * @return
 * 		zero if successful, otherwise non-zero
 */
int fsetpos(FILE* stream, const fpos_t* pos);

/**
 * Obtains the current value of the file position indicator. (N1548-7.21.9.4)
 *
 * @param stream
 * 		stream to get position from
 * @return
 * 		current position if successful, otherwise -1L
 */
long int ftell(FILE* stream);
off_t ftello(FILE* stream);

/**
 * Resets the <stream> position to zero and clears the error indicator. (N1548-7.21.9.5)
 *
 * @param stream
 * 		stream to reset
 */
void rewind(FILE* stream);

/**
 * Resets the <stream>s error indicator. (N1548-7.21.10.1)
 *
 * @param stream
 * 		the stream
 */
void clearerr(FILE* stream);

/**
 * Sets the <stream>s error indicator. (N1548-7.21.10.1)
 *
 * @param stream
 * 		the stream
 */
void fseterr(FILE* stream);

/**
 * Tests whether end-of-file indicator of the stream is set. (N1548-7.21.10.2)
 *
 * @param stream
 * 		the stream
 * @return
 * 		non-zero if end-of-file indicator is set
 */
int feof(FILE* stream);

/**
 * Tests whether error indicator of the stream is set. (N1548-7.21.10.3)
 *
 * @param stream
 * 		the stream
 * @return
 * 		non-zero if error indicator is set
 */
int ferror(FILE *stream);

/**
 * Maps the error code in <errno> to a message; then prints the
 * given string <s> (if not NULL), followed by a colon ':' and space ' ',
 * followed by the error message and a new-line to the standard error stream. (N1548-7.21.10.4)
 *
 * @param s
 * 		message to prepend, or NULL
 */
void perror(const char* s);

/**
 * Determines the file descriptor for a stream. (POSIX)
 *
 * @param stream
 * 		the stream
 * @return
 * 		associated file descriptor
 */
int fileno(FILE* stream);

/**
 * Opens a stream from a file descriptor.
 *
 * @param fildes
 * 		file descriptor
 *
 */
FILE* fdopen(int fd, const char *mode);

/**
 * Formatted writing to the kernel log.
 *
 * @param format
 * 		format string to use
 * @varargs
 * 		optional argument list
 */
void klog(const char* format, ...);

/**
 * Formatted writing to the kernel log.
 *
 * @param format
 * 		format string to use
 * @param args
 * 		argument list
 */
void kvlog(const char* format, va_list args);

/**
 * TODO
 */
FILE* popen(const char* command, const char* mode);

/**
 * TODO
 */
int pclose(FILE* stream);

__END_C

#endif
