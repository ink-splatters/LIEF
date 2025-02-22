/* Copyright 2017 - 2023 R. Thomas
 * Copyright 2017 - 2023 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "pyOAT.hpp"

#include "LIEF/OAT/Parser.hpp"

#include <string>

namespace LIEF {
namespace OAT {

template<>
void create<Parser>(py::module& m) {

  // Parser (Parser)
  m.def("parse",
    static_cast<std::unique_ptr<Binary> (*) (const std::string&)>(&Parser::parse),
    "Parse the given OAT file and return a " RST_CLASS_REF(lief.OAT.Binary) " object",
    "oat_file"_a,
    py::return_value_policy::take_ownership);

  m.def("parse",
    static_cast<std::unique_ptr<Binary> (*) (const std::string&, const std::string&)>(&Parser::parse),
    "Parse the given OAT with its VDEX file and return a " RST_CLASS_REF(lief.OAT.Binary) " object",
    "oat_file"_a, "vdex_file"_a,
    py::return_value_policy::take_ownership);

  m.def("parse",
    static_cast<std::unique_ptr<Binary> (*) (std::vector<uint8_t>)>(&Parser::parse),
    "Parse the given raw data and return a " RST_CLASS_REF(lief.OAT.Binary) " object",
    "raw"_a,
    py::return_value_policy::take_ownership);


  m.def("parse",
      [] (py::object byteio) {
        const auto& io = py::module::import("io");
        const auto& RawIOBase = io.attr("RawIOBase");
        const auto& BufferedIOBase = io.attr("BufferedIOBase");
        const auto& TextIOBase = io.attr("TextIOBase");

        py::object rawio;


        if (py::isinstance(byteio, RawIOBase)) {
          rawio = byteio;
        }

        else if (py::isinstance(byteio, BufferedIOBase)) {
          rawio = byteio.attr("raw");
        }

        else if (py::isinstance(byteio, TextIOBase)) {
          rawio = byteio.attr("buffer").attr("raw");
        }

        else {
          throw py::type_error(py::repr(byteio).cast<std::string>().c_str());
        }

        std::string raw_str = static_cast<py::bytes>(rawio.attr("readall")());
        std::vector<uint8_t> raw = {
          std::make_move_iterator(std::begin(raw_str)),
          std::make_move_iterator(std::end(raw_str))};

        return LIEF::OAT::Parser::parse(std::move(raw));
      },
      "io"_a,
      py::return_value_policy::take_ownership);
}
}
}
