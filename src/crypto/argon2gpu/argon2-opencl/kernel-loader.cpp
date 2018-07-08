/*
 * Copyright (C) 2015-2018 Łukasz Kurowski <crackcomm@gmail.com>, Ondrej Mosnacek <omosnacek@gmail.com>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation: either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "crypto/argon2gpu/argon2-opencl/kernel-loader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace argon2gpu
{
namespace opencl
{

cl::Program KernelLoader::loadArgon2Program(
    const cl::Context &context,
    const std::string &sourceDirectory,
    Type type, Version version, bool debug)
{
    std::string sourcePath = sourceDirectory + "/kernel.cl";
    std::string sourceText;
    std::stringstream buildOpts;
    {
        std::ifstream sourceFile{sourcePath};
        sourceText = {
            std::istreambuf_iterator<char>(sourceFile),
            std::istreambuf_iterator<char>()};
    }

    if (debug)
    {
        buildOpts << "-g -s \"" << sourcePath << "\""
                  << " ";
    }
    buildOpts << "-DARGON2_TYPE=" << type << " ";
    buildOpts << "-DARGON2_VERSION=" << version << " ";

    cl::Program prog(context, sourceText);
    try
    {
        std::string opts = buildOpts.str();
        prog.build(opts.c_str());
    }
    catch (const cl::Error &err)
    {
        std::cerr << "ERROR: Failed to build program:" << std::endl;
        for (cl::Device &device : context.getInfo<CL_CONTEXT_DEVICES>())
        {
            std::cerr << "  Build log from device '" << device.getInfo<CL_DEVICE_NAME>() << "':" << std::endl;
            std::cerr << prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        }
        throw;
    }
    return prog;
}

} // namespace opencl
} // namespace argon2gpu
