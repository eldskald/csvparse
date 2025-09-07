// https://github.com/eldskald/csvparse
//
// MIT License
//
// Copyright (c) 2025 Rafael de Lima Bordoni
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

// Parses the file at 'filepath' and returns the data, setting 'size' to the
// data's array size. If an error occurs, returns NULL and sets 'err' to
// indicate the error.
// WARNING: data must be freed to avoid memory leakage. Call csvfree(data) when
// done with it.
// 'err' = 0 means no error.
// 'err' = 1 means file opening error, use errno for more details.
// 'err' = 2 means a cell starts with " and doesn't end with it.
// 'err' = 3 means a cell is too big.
// 'err' = 4 means a row has too many cells.
// 'err' = 5 means too many rows.
const char ***csvparse(const char *filepath, int *err);

// Returns the total rows contained in the parsed csv.
unsigned int csvrowscount(const char ***data);

// Returns the size of a row in the parsed csv.
unsigned int csvrowlen(const char **data);

// Frees the memory of the parsed csv data.
void csvfree(const char ***data);
