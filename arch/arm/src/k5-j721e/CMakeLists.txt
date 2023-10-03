# ##############################################################################
# arch/arm/src/k5-j721e/CMakeLists.txt
#
# Licensed to the Apache Software Foundation (ASF) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  The ASF licenses this
# file to you under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#
# ##############################################################################

set(SRCS
    k5_j721e_boot.c
    k5_j721e_clockconfig.c
    k5_j721e_esm.c
    k5_j721e_gio.c
    k5_j721e_irq.c
    k5_j721e_lowputc.c
    k5_j721e_serial.c)

if(NOT CONFIG_SCHED_TICKLESS)
  list(APPEND SRCS k5_j721e_timerisr.c)
endif()

if(CONFIG_K5_J721E_SELFTEST)
  list(APPEND SRCS k5_j721e_selftest.c)
endif()

if(CONFIG_K5_J721E_GIO_IRQ)
  list(APPEND SRCS k5_j721e_gioirq.c)
endif()

target_sources(arch PRIVATE ${SRCS})
