#include "t_shell.h"

/**
 * bufferInput - buffers chained commands
 * @info: parameter struct
 * @buffer: address of buffer
 * @length: address of len var
 *
 * Return: bytes
 */
ssize_t bufferInput(info_t *info, char **buffer, size_t *length)
{
	ssize_t bytesRead = 0;
	size_t currentLength = 0;

	if (!*length)
	{
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, interruptHandler);
#if USE_GETLINE
		bytesRead = getline(buffer, &currentLength, stdin);
#else
		bytesRead = _getline(info, buffer, &currentLength);
#endif
		if (bytesRead > 0)
		{
			if ((*buffer)[bytesRead - 1] == '\n')
			{
				(*buffer)[bytesRead - 1] = '\0';
				bytesRead--;
			}
			info->lineCountFlag = 1;
			erase_comments(*buffer);
			buildHistoryList(info, *buffer, info->histcount++);
			*length = bytesRead;
			info->cmdBuffer = buffer;
		}
	}
	return (bytesRead);
}

/**
 * getInput - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t getInput(info_t *info)
{
	static char *buffer;
	static size_t i, j, length;
	ssize_t bytesRead = 0;
	char **bufferPointer = &(info->arg), *ptr;

	_putchar(BUFFER_FLUSH);
	bytesRead = bufferInput(info, &buffer, &length);
	if (bytesRead == -1)
		return (-1);
	if (length)
	{
		j = i;
		ptr = buffer + i;
		checkCommandChain(info, buffer, &j, i, length);
		while (j < length)
		{
			if (isCommandChain(info, buffer, &j))
				break;
			j++;
		}

		i = j + 1;
		if (i >= length)
		{
			i = length = 0;
			info->commandBufferType = COMMAND_NORMAL;
		}

		*bufferPointer = ptr;
		return (_strlen(ptr));
	}

	*bufferPointer = buffer;
	return (bytesRead);
}

/**
 * readBuffer - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: bytesRead
 */
ssize_t readBuffer(info_t *info, char *buf, size_t *i)
{
	ssize_t bytesRead = 0;

	if (*i)
		return (0);
	bytesRead = read(info->readFd, buf, READ_BUFFER_SIZE);
	if (bytesRead >= 0)
		*i = bytesRead;
	return (bytesRead);
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: bytesRead
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUFFER_SIZE];
	static size_t bufferIndex, bufferLength;
	size_t k;
	ssize_t bytesRead = 0, bytes = 0;
	char *currBuf = NULL, *newBuffer = NULL, *c;

	currBuf = *ptr;
	if (currBuf && length)
		bytes = *length;
	if (bufferIndex == bufferLength)
		bufferIndex = bufferLength = 0;

	bytesRead = readBuffer(info, buf, &bufferLength);
	if (bytesRead == -1 || (bytesRead == 0 && bufferLength == 0))
		return (-1);

	c = _strchr(buf + bufferIndex, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : bufferLength;
	newBuffer = _realloc(currBuf, bytes, bytes ? bytes + k : k + 1);
	if (!newBuffer)
		return (currBuf ? free(currBuf), -1 : -1);

	if (bytes)
		_strncat(newBuffer, buf + bufferIndex, k - bufferIndex);
	else
		_strncpy(newBuffer, buf + bufferIndex, k - bufferIndex + 1);

	bytes += k - bufferIndex;
	bufferIndex = k;
	currBuf = newBuffer;

	if (length)
		*length = bytes;
	*ptr = currBuf;
	return (bytes);
}

/**
 * interruptHandler - blocks ctrl-C
 * @signalNumber: the signal number
 *
 * Return: nothing
 */
void interruptHandler(__attribute__((unused))int signalNumber)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUFFER_FLUSH);
}



