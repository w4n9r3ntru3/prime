/***********************************************************************

  FileName    [mmapstream.h]

  Author      [Kan-Hsuan, Lai]

  This file describes the functions in "mmapstream.h".

***********************************************************************/

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <assert.h>

#include "mmapstream.h"

mstream::mstream(const char* filepath) {
    int fd = open(filepath, O_RDONLY);

    if (fd == -1) {
        _is_opened = false;
        return;
    }

    struct stat buf;
    fstat(fd, &buf);
    _max_size = buf.st_size;
    _content = (char*)mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (_content == MAP_FAILED) {
        _is_opened = false;
        return;
    }

    _is_opened = true;
    _current = 0;
}

mstream& mstream::operator>>(std::string& dest) {
    assert(_current < _max_size);
    while ((_content[_current] == '\n' || _content[_current] == ' ') &&
           _current < _max_size) {
        _current++;
    }
    assert(_current < _max_size);
    int base = _current;
    int length = 0;
    while ((_content[_current] != '\n' && _content[_current] != ' ') &&
           _current < _max_size) {
        _current++, length++;
    }
    assert(_current < _max_size);

    dest.clear();
    dest.reserve(length + 1);
    for (int i = 0; i < length; i++) {
        dest.push_back(_content[base + i]);
    }

    return *this;
}

mstream& mstream::operator>>(unsigned& dest) {
    assert(_current < _max_size);
    while ((_content[_current] == '\n' || _content[_current] == ' ') &&
           _current < _max_size) {
        _current++;
    }
    assert(_current < _max_size);
    int base = _current;
    int length = 0;
    while ((_content[_current] != '\n' && _content[_current] != ' ') &&
           _current < _max_size) {
        _current++;
        length++;
    }
    assert(_current < _max_size);

    dest = 0;
    for (int i = 0; i < length; i++) {
        dest *= 10;
        assert(_content[base + i] >= '0' && _content[base + i] <= '9');
        dest += _content[base + i] - '0';
    }

    return *this;
}

mstream& mstream::operator>>(int& dest) {
    assert(_current < _max_size);
    while ((_content[_current] == '\n' || _content[_current] == ' ') &&
           _current < _max_size) {
        _current++;
    }
    assert(_current < _max_size);
    int base = _current;
    int length = 0;
    while ((_content[_current] != '\n' && _content[_current] != ' ') &&
           _current < _max_size) {
        _current++;
        length++;
    }
    assert(_current < _max_size);

    dest = 0;
    int sign = 1;
    if (_content[base] == '-') {
        sign = -1;
        base++;
        length--;
    } else if (_content[base] == '+') {
        base++;
        length--;
    }

    for (int i = 0; i < length; i++) {
        dest *= 10;
        assert(_content[base + i] >= '0' && _content[base + i] <= '9');
        dest += _content[base + i] - '0';
    }

    dest *= sign;

    return *this;
}

bool mstream::is_open() const {
    return _is_opened;
}
