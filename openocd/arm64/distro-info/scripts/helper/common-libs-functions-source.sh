# -----------------------------------------------------------------------------
# This file is part of the xPack distribution.
#   (https://xpack.github.io)
# Copyright (c) 2020 Liviu Ionescu.
#
# Permission to use, copy, modify, and/or distribute this software 
# for any purpose is hereby granted, under the terms of the MIT license.
# -----------------------------------------------------------------------------

# Helper script used in the second edition of the xPack build 
# scripts. As the name implies, it should contain only functions and 
# should be included with 'source' by the build scripts (both native
# and container).

# -----------------------------------------------------------------------------

function build_zlib() 
{
  # http://zlib.net
  # http://zlib.net/fossils/

  # https://archlinuxarm.org/packages/aarch64/zlib/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=zlib-static
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=zlib-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-zlib

  # 2013-04-28 "1.2.8"
  # 2017-01-15 "1.2.11"

  local zlib_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local zlib_src_folder_name="zlib-${zlib_version}"

  local zlib_archive="${zlib_src_folder_name}.tar.gz"
  local zlib_url="http://zlib.net/fossils/${zlib_archive}"

  # The folder name for build, licenses, etc.
  local zlib_folder_name="${zlib_src_folder_name}"

  local zlib_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${zlib_folder_name}-installed"
  if [ ! -f "${zlib_stamp_file_path}" ]
  then

    # In-source build.

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${zlib_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      download_and_extract "${zlib_url}" "${zlib_archive}" \
        "${zlib_src_folder_name}"

      if [ "${zlib_src_folder_name}" != "${zlib_folder_name}" ]
      then
        mv -v "${zlib_src_folder_name}" "${zlib_folder_name}"
      fi
    fi

    mkdir -pv "${LOGS_FOLDER_PATH}/${zlib_folder_name}"

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${zlib_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      if [ "${TARGET_PLATFORM}" == "win32" ]
      then
        (
          echo
          echo "Running zlib make..."

          # Build.
          run_verbose make -f win32/Makefile.gcc \
            PREFIX=${CROSS_COMPILE_PREFIX}- \
            prefix="${LIBS_INSTALL_FOLDER_PATH}" \
            CFLAGS="${XBB_CFLAGS_NO_W} -Wp,-D_FORTIFY_SOURCE=2 -fexceptions --param=ssp-buffer-size=4"
          
          run_verbose make -f win32/Makefile.gcc install \
            DESTDIR="${LIBS_INSTALL_FOLDER_PATH}/" \
            INCLUDE_PATH="include" \
            LIBRARY_PATH="lib" \
            BINARY_PATH="bin"

        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zlib_folder_name}/make-output.txt"
      else

        CPPFLAGS="${XBB_CPPFLAGS}"
        CFLAGS="${XBB_CFLAGS_NO_W}"
        CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
        LDFLAGS="${XBB_LDFLAGS_LIB}"
        if [ "${IS_DEVELOP}" == "y" ]
        then
          LDFLAGS+=" -v"
        fi

        export CPPFLAGS
        export CFLAGS
        export CXXFLAGS
        export LDFLAGS

        env | sort

        # No config.status left, use the library.
        if [ ! -f "libz.a" ]
        then
          (
            echo
            echo "Running zlib configure..."

            bash "configure" --help

            run_verbose bash ${DEBUG} "configure" \
              --prefix="${LIBS_INSTALL_FOLDER_PATH}" 
            
            cp "configure.log" "${LOGS_FOLDER_PATH}/${zlib_folder_name}/configure-log.txt"
          ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zlib_folder_name}/configure-output.txt"
        fi

        (
          echo
          echo "Running zlib make..."

          # Build.
          run_verbose make -j ${JOBS}

          if [ "${WITH_TESTS}" == "y" ]
          then
            run_verbose make test
          fi

          run_verbose make install

        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zlib_folder_name}/make-output.txt"
      fi

      copy_license \
        "${LIBS_BUILD_FOLDER_PATH}/${zlib_folder_name}" \
        "${zlib_folder_name}"

    )

    touch "${zlib_stamp_file_path}"

  else
    echo "Library zlib already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_gmp() 
{
  # https://gmplib.org
  # https://gmplib.org/download/gmp/

  # https://archlinuxarm.org/packages/aarch64/gmp/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=gmp-hg

  # 01-Nov-2015 "6.1.0"
  # 16-Dec-2016 "6.1.2"
  # 17-Jan-2020 "6.2.0"

  local gmp_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local gmp_src_folder_name="gmp-${gmp_version}"

  local gmp_archive="${gmp_src_folder_name}.tar.xz"
  local gmp_url="https://gmplib.org/download/gmp/${gmp_archive}"

  # The folder name for build, licenses, etc.
  local gmp_folder_name="${gmp_src_folder_name}"

  local gmp_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${gmp_folder_name}-installed"
  if [ ! -f "${gmp_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${gmp_url}" "${gmp_archive}" \
      "${gmp_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${gmp_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${gmp_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${gmp_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      # ABI is mandatory, otherwise configure fails on 32-bit.
      # (see https://gmplib.org/manual/ABI-and-ISA.html)
      if [ "${TARGET_ARCH}" == "x64" -o "${TARGET_ARCH}" == "x32" ]
      then
        export ABI="${TARGET_BITS}"
      fi

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running gmp configure..."

          # ABI is mandatory, otherwise configure fails on 32-bit.
          # (see https://gmplib.org/manual/ABI-and-ISA.html)

          bash "${SOURCES_FOLDER_PATH}/${gmp_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")
            
          config_options+=("--enable-cxx")

          if [ "${TARGET_PLATFORM}" == "win32" ]
          then
            # mpfr asks for this explicitly during configure.
            # (although the message is confusing)
            config_options+=("--enable-shared")
            config_options+=("--disable-static")
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${gmp_src_folder_name}/configure" \
            ${config_options[@]}
            
          cp "config.log" "${LOGS_FOLDER_PATH}/${gmp_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gmp_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running gmp make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gmp_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${gmp_src_folder_name}" \
        "${gmp_folder_name}"

    )

    touch "${gmp_stamp_file_path}"

  else
    echo "Library gmp already installed."
  fi
}

function build_mpfr()
{
  # http://www.mpfr.org
  # http://www.mpfr.org/history.html

  # https://archlinuxarm.org/packages/aarch64/mpfr/files/PKGBUILD
  # https://git.archlinux.org/svntogit/packages.git/tree/trunk/PKGBUILD?h=packages/mpfr

  # 6 March 2016 "3.1.4"
  # 7 September 2017 "3.1.6"
  # 31 January 2019 "4.0.2"

  local mpfr_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local mpfr_src_folder_name="mpfr-${mpfr_version}"

  local mpfr_archive="${mpfr_src_folder_name}.tar.xz"
  local mpfr_url="http://www.mpfr.org/${mpfr_src_folder_name}/${mpfr_archive}"

  # The folder name for build, licenses, etc.
  local mpfr_folder_name="${mpfr_src_folder_name}"

  local mpfr_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mpfr_folder_name}-installed"
  if [ ! -f "${mpfr_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${mpfr_url}" "${mpfr_archive}" \
      "${mpfr_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${mpfr_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${mpfr_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${mpfr_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running mpfr configure..."

          bash "${SOURCES_FOLDER_PATH}/${mpfr_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--disable-warnings")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${mpfr_src_folder_name}/configure" \
            ${config_options[@]}
             
          cp "config.log" "${LOGS_FOLDER_PATH}/${mpfr_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${mpfr_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running mpfr make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${mpfr_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${mpfr_src_folder_name}" \
        "${mpfr_folder_name}"

    )
    touch "${mpfr_stamp_file_path}"

  else
    echo "Library mpfr already installed."
  fi
}

function build_mpc()
{
  # http://www.multiprecision.org/
  # ftp://ftp.gnu.org/gnu/mpc

  # https://archlinuxarm.org/packages/aarch64/mpc/files/PKGBUILD
  # https://git.archlinux.org/svntogit/packages.git/tree/trunk/PKGBUILD?h=packages/libmpc

  # 20 Feb 2015 "1.0.3"
  # 2018-01-11 "1.1.0"

  local mpc_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local mpc_src_folder_name="mpc-${mpc_version}"

  local mpc_archive="${mpc_src_folder_name}.tar.gz"
  local mpc_url="ftp://ftp.gnu.org/gnu/mpc/${mpc_archive}"
  if [[ "${mpc_version}" =~ 0\.* ]]
  then
    mpc_url="http://www.multiprecision.org/downloads/${mpc_archive}"
  fi

  # The folder name for build, licenses, etc.
  local mpc_folder_name="${mpc_src_folder_name}"

  local mpc_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${mpc_folder_name}-installed"
  if [ ! -f "${mpc_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${mpc_url}" "${mpc_archive}" \
      "${mpc_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${mpc_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${mpc_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${mpc_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running mpc configure..."
        
          bash "${SOURCES_FOLDER_PATH}/${mpc_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${mpc_src_folder_name}/configure" \
            ${config_options[@]}
            
          cp "config.log" "${LOGS_FOLDER_PATH}/${mpc_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${mpc_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running mpc make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${mpc_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${mpc_src_folder_name}" \
        "${mpc_folder_name}"

    )
    touch "${mpc_stamp_file_path}"

  else
    echo "Library mpc already installed."
  fi
}

function build_isl()
{
  # http://isl.gforge.inria.fr

  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=isl

  # 2015-06-12 "0.15"
  # 2016-01-15 "0.16.1"
  # 2016-12-20 "0.18"
  # 2019-03-26 "0.21"
  # 2020-01-16 "0.22"

  local isl_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local isl_src_folder_name="isl-${isl_version}"

  local isl_archive="${isl_src_folder_name}.tar.xz"
  if [[ "${isl_version}" =~ 0\.12\.* ]]
  then
    isl_archive="${isl_src_folder_name}.tar.gz"
  fi

  local isl_url="http://isl.gforge.inria.fr/${isl_archive}"

  # The folder name for build, licenses, etc.
  local isl_folder_name="${isl_src_folder_name}"

  local isl_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${isl_folder_name}-installed"
  if [ ! -f "${isl_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${isl_url}" "${isl_archive}" \
      "${isl_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${isl_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${isl_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${isl_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running isl configure..."

          bash "${SOURCES_FOLDER_PATH}/${isl_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${isl_src_folder_name}/configure" \
            ${config_options[@]}
            
          cp "config.log" "${LOGS_FOLDER_PATH}/${isl_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${isl_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running isl make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          if [ "${TARGET_PLATFORM}" == "linux" -a \
            \( "${TARGET_ARCH}" == "x64" -o "${TARGET_ARCH}" == "x32" \) ] 
          then
            # /Host/Users/ilg/Work/gcc-8.4.0-1/linux-x64/build/libs/isl-0.22/.libs/lt-isl_test_cpp: relocation error: /Host/Users/ilg/Work/gcc-8.4.0-1/linux-x64/build/libs/isl-0.22/.libs/lt-isl_test_cpp: symbol _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm, version GLIBCXX_3.4.21 not defined in file libstdc++.so.6 with link time reference
            # FAIL isl_test_cpp (exit status: 127)
            # /Host/Users/ilg/Work/gcc-8.4.0-1/linux-x32/build/libs/isl-0.22/.libs/lt-isl_test_cpp: relocation error: /Host/Users/ilg/Work/gcc-8.4.0-1/linux-x32/build/libs/isl-0.22/.libs/lt-isl_test_cpp: symbol _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERjj, version GLIBCXX_3.4.21 not defined in file libstdc++.so.6 with link time reference
            # FAIL isl_test_cpp (exit status: 127)

            run_verbose make check || true
          else
            run_verbose make check
          fi
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${isl_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${isl_src_folder_name}" \
        "${isl_folder_name}"

    )
    touch "${isl_stamp_file_path}"

  else
    echo "Library isl already installed."
  fi
}

function build_zstd()
{
  # https://facebook.github.io/zstd/
  # https://github.com/facebook/zstd/releases
  # https://github.com/facebook/zstd/archive/v1.4.4.tar.gz

  # https://archlinuxarm.org/packages/aarch64/zstd/files/PKGBUILD

  # 5 Nov 2019 "1.4.4"

  local zstd_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local zstd_src_folder_name="zstd-${zstd_version}"

  local zstd_archive="${zstd_src_folder_name}.tar.gz"

  # GitHub release archive.
  local zstd_github_archive="v${zstd_version}.tar.gz"

  local zstd_url="https://github.com/facebook/zstd/archive/${zstd_github_archive}"

  # The folder name for build, licenses, etc.
  local zstd_folder_name="${zstd_src_folder_name}"

  local zstd_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${zstd_folder_name}-installed"
  if [ ! -f "${zstd_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${zstd_url}" "${zstd_archive}" \
      "${zstd_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${zstd_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${zstd_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${zstd_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      local build_type
      if [ "${IS_DEBUG}" == "y" ]
      then
        build_type=Debug
      else
        build_type=Release
      fi

      env | sort

      if [ ! -f "CMakeCache.txt" ]
      then 
        (
          echo
          echo "Running zstd cmake..."
        
          config_options=()

          config_options+=("-LH")
          config_options+=("-G" "Ninja")

          config_options+=("-DCMAKE_INSTALL_PREFIX=${LIBS_INSTALL_FOLDER_PATH}")
          config_options+=("-DZSTD_BUILD_PROGRAMS=OFF")

          if [ "${WITH_TESTS}" == "y" ]
          then
            config_options+=("-DZSTD_BUILD_TESTS=ON")
          fi

          run_verbose cmake \
            ${config_options[@]} \
            \
            "${SOURCES_FOLDER_PATH}/${zstd_src_folder_name}/build/cmake"
            
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zstd_folder_name}/zstd-output.txt"
      fi

      (
        echo
        echo "Running zstd build..."

        run_verbose cmake \
          --build . \
          --parallel ${JOBS} \
          --config "${build_type}" \

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose ctest \
            -V \

        fi

        (
          # The install procedure runs some resulted exxecutables, which require
          # the libssl and libcrypt libraries from XBB.
          # xbb_activate_libs

          echo
          echo "Running zstd install..."

          run_verbose cmake \
            --build . \
            --config "${build_type}" \
            -- \
            install

        )
      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${zstd_folder_name}/build-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${zstd_src_folder_name}" \
        "${zstd_folder_name}"

      (
        cd "${LIBS_BUILD_FOLDER_PATH}"

        copy_cmake_logs "${zstd_folder_name}"
      )

    )
    touch "${zstd_stamp_file_path}"

  else
    echo "Library zstd already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libiconv()
{
  # https://www.gnu.org/software/libiconv/
  # https://ftp.gnu.org/pub/gnu/libiconv/

  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=libiconv

  # 2011-08-07 1.14"
  # 2017-02-02 "1.15"
  # 2019-04-26 "1.16"

  local libiconv_version="$1"

  local libiconv_src_folder_name="libiconv-${libiconv_version}"

  local libiconv_archive="${libiconv_src_folder_name}.tar.gz"
  local libiconv_url="https://ftp.gnu.org/pub/gnu/libiconv/${libiconv_archive}"

  local libiconv_folder_name="${libiconv_src_folder_name}"

  local libiconv_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${libiconv_folder_name}-installed"
  if [ ! -f "${libiconv_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libiconv_url}" "${libiconv_archive}" \
      "${libiconv_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${libiconv_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libiconv_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libiconv_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      # -fgnu89-inline fixes "undefined reference to `aliases2_lookup'"
      #  https://savannah.gnu.org/bugs/?47953
      CFLAGS="${XBB_CFLAGS_NO_W} -fgnu89-inline"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running libiconv configure..."

          bash "${SOURCES_FOLDER_PATH}/${libiconv_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libiconv_src_folder_name}/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${libiconv_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libiconv_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running libiconv make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libiconv_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libiconv_src_folder_name}" \
        "${libiconv_folder_name}"

    )

    touch "${libiconv_stamp_file_path}"

  else
    echo "Library libiconv already installed."
  fi
}

# -----------------------------------------------------------------------------

# Could not make it work on Windows.
function build_ncurses()
{
  # https://invisible-island.net/ncurses/
  # ftp://ftp.invisible-island.net/pub/ncurses
  # ftp://ftp.invisible-island.net/pub/ncurses/ncurses-6.2.tar.gz

  # depends=(glibc gcc-libs)
  # https://archlinuxarm.org/packages/aarch64/ncurses/files/PKGBUILD
  # http://deb.debian.org/debian/pool/main/n/ncurses/ncurses_6.1+20181013.orig.tar.gz.asc

  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-ncurses/PKGBUILD
  # https://github.com/msys2/MINGW-packages/blob/master/mingw-w64-ncurses/001-use-libsystre.patch
  # https://github.com/msys2/MSYS2-packages/blob/master/ncurses/PKGBUILD

  # _4421.c:1364:15: error: expected ‘)’ before ‘int’
  # ../include/curses.h:1906:56: note: in definition of macro ‘mouse_trafo’
  # 1906 | #define mouse_trafo(y,x,to_screen) wmouse_trafo(stdscr,y,x,to_screen)

  # 26 Feb 2011, "5.8" # build fails
  # 27 Jan 2018, "5.9" # build fails
  # 27 Jan 2018, "6.1"
  # 12 Feb 2020, "6.2"

  local ncurses_version="$1"

  # The folder name as resulted after being extracted from the archive.
  local ncurses_src_folder_name="ncurses-${ncurses_version}"

  local ncurses_archive="${ncurses_src_folder_name}.tar.gz"
  local ncurses_url="ftp://ftp.invisible-island.net//pub/ncurses/${ncurses_archive}"

  # The folder name  for build, licenses, etc.
  local ncurses_folder_name="${ncurses_src_folder_name}"

  # local ncurses_version_major="$(echo ${ncurses_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)|\1|')"
  # local ncurses_version_minor="$(echo ${ncurses_version} | sed -e 's|\([0-9][0-9]*\)\.\([0-9][0-9]*\)|\2|')"

  local ncurses_stamp_file_path="${STAMPS_FOLDER_PATH}/stamp-${ncurses_folder_name}-installed"
  if [ ! -f "${ncurses_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${ncurses_url}" "${ncurses_archive}" \
      "${ncurses_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${ncurses_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${ncurses_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${ncurses_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running ncurses configure..."

          bash "${SOURCES_FOLDER_PATH}/${ncurses_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # Not yet functional on windows.
          if [ "${TARGET_PLATFORM}" == "win32" ]
          then

            # The build passes, but generally it is not expected to be
            # used on Windows.

            # export PATH_SEPARATOR=";"

            # --with-libtool \
            # /opt/xbb/bin/libtool: line 10548: gcc-8bs: command not found

            # Without --with-pkg-config-libdir= it'll try to write the .pc files in the
            # xbb folder, probbaly by using the dirname of pkg-config.

            config_options+=("--with-build-cc=${NATIVE_CC}")
            config_options+=("--with-build-cflags=${CFLAGS}")
            config_options+=("--with-build-cppflags=${CPPFLAGS}")
            config_options+=("--with-build-ldflags=${LDFLAGS}")
               
            config_options+=("--without-progs")

            # Only for the MinGW port, it provides a way to substitute
            # the low-level terminfo library with different terminal drivers.
            config_options+=("--enable-term-driver")
            
            config_options+=("--disable-termcap")
            config_options+=("--disable-home-terminfo")
            config_options+=("--disable-db-install")

          else

            # Without --with-pkg-config-libdir= it'll try to write the .pc files in the
            # xbb folder, probbaly by using the dirname of pkg-config.

            config_options+=("--with-terminfo-dirs=/etc/terminfo")
            config_options+=("--with-default-terminfo-dir=/etc/terminfo:/lib/terminfo:/usr/share/terminfo")
            config_options+=("--with-gpm")
            config_options+=("--with-versioned-syms")
            config_options+=("--with-xterm-kbs=del")

            config_options+=("--enable-termcap")
            config_options+=("--enable-const")
            config_options+=("--enable-symlinks")

          fi

          config_options+=("--with-shared")
          config_options+=("--with-normal")
          config_options+=("--with-cxx")
          config_options+=("--with-cxx-binding")
          config_options+=("--with-cxx-shared")
          config_options+=("--with-pkg-config-libdir=${LIBS_INSTALL_FOLDER_PATH}/lib/pkgconfig")
          
          # Fails on Linux, with missing _nc_cur_term, which is there.
          config_options+=("--without-pthread")

          config_options+=("--without-ada")
          config_options+=("--without-debug")
          config_options+=("--without-manpages")
          config_options+=("--without-tack")
          config_options+=("--without-tests")

          config_options+=("--enable-pc-files")
          config_options+=("--enable-sp-funcs")
          config_options+=("--enable-ext-colors")
          config_options+=("--enable-interop")

          config_options+=("--disable-lib-suffixes")
          config_options+=("--disable-overwrite")

          NCURSES_DISABLE_WIDEC=${NCURSES_DISABLE_WIDEC:-""}

          if [ "${NCURSES_DISABLE_WIDEC}" == "y" ]
          then
            config_options+=("--disable-widec")
          fi

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${ncurses_src_folder_name}/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${ncurses_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${ncurses_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running ncurses make..."

        # Build.
        run_verbose make -j ${JOBS}

        # The test-programs are interactive

        # make install-strip
        run_verbose make install

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${ncurses_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${ncurses_src_folder_name}" \
        "${ncurses_folder_name}"

    )

    touch "${ncurses_stamp_file_path}"

  else
    echo "Library ncurses already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libffi() 
{
  # http://www.sourceware.org/libffi/
  # ftp://sourceware.org/pub/libffi/
  # https://github.com/libffi/libffi/archive/v3.2.1.tar.gz

  # https://archlinuxarm.org/packages/aarch64/libffi/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=libffi-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-libffi

  # 12-Nov-2014, "3.2.1"
  # 23 Nov 2019, "3.3"

  local libffi_version="$1"

  local libffi_src_folder_name="libffi-${libffi_version}"

  local libffi_archive="${libffi_src_folder_name}.tar.gz"
  # local libffi_url="ftp://sourceware.org/pub/libffi/${libffi_archive}"
  # GitHub release archive.
  local libffi_url="https://github.com/libffi/libffi/archive/v${libffi_version}.tar.gz"

  local libffi_folder_name="${libffi_src_folder_name}"

  local libffi_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${libffi_folder_name}-installed"
  if [ ! -f "${libffi_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libffi_url}" "${libffi_archive}" \
      "${libffi_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${libffi_folder_name}"

    (
      if [ ! -x "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}/configure" ]
      then

        cd "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}"
        
        xbb_activate
        xbb_activate_installed_dev

        run_verbose bash ${DEBUG} "autogen.sh"

      fi
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libffi_folder_name}/autogen-output.txt"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libffi_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libffi_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 

        (
          echo
          echo "Running libffi configure..."

          bash "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          config_options+=("--enable-pax_emutramp")

          # --enable-pax_emutramp is inspired by AUR
          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${libffi_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libffi_folder_name}/configure-output.txt"

      fi

      (
        echo
        echo "Running libffi make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libffi_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libffi_src_folder_name}" \
        "${libffi_folder_name}"

    )

    touch "${libffi_stamp_file_path}"

  else
    echo "Library libffi already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_gettext() 
{
  # https://www.gnu.org/software/gettext/
  # http://ftp.gnu.org/pub/gnu/gettext/

  # https://archlinuxarm.org/packages/aarch64/gettext/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=gettext-git
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=mingw-w64-gettext

  # gettext_version="0.19.5.1"
  # gettext_version="0.19.8.1" # 2016-06-11

  local gettext_version="$1"

  local gettext_src_folder_name="gettext-${gettext_version}"

  local gettext_archive="${gettext_src_folder_name}.tar.gz"
  local gettext_url="http://ftp.gnu.org/pub/gnu/gettext/${gettext_archive}"

  local gettext_folder_name="${gettext_src_folder_name}"
  local gettext_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${gettext_folder_name}-installed"
  if [ ! -f "${gettext_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${gettext_url}" "${gettext_archive}" \
      "${gettext_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${gettext_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${gettext_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${gettext_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"      
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 

        (
          echo
          echo "Running gettext configure..."

          # Build only the /gettext-runtime folder, attempts to build
          # the full package fail with a CXX='no' problem.
          bash "${SOURCES_FOLDER_PATH}/${gettext_src_folder_name}/gettext-runtime/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          if [ "${TARGET_PLATFORM}" == "win32" ]
          then
            config_options+=("--enable-threads=windows")
            config_options+=("--with-gnu-ld")
          elif [ "${TARGET_PLATFORM}" == "linux" ]
          then
            config_options+=("--enable-threads=posix")
            config_options+=("--with-gnu-ld")
          elif [ "${TARGET_PLATFORM}" == "darwin" ]
          then
            config_options+=("--enable-threads=posix")
          fi

          config_options+=("--disable-installed-tests")
          config_options+=("--disable-always-build-tests")
          # config_options+=("--enable-nls")
          config_options+=("--disable-rpath")
          config_options+=("--disable-java")
          config_options+=("--disable-native-java")
          config_options+=("--disable-c++")
          config_options+=("--disable-libasprintf")

          #  --enable-nls needed to include libintl
          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${gettext_src_folder_name}/gettext-runtime/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${gettext_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gettext_folder_name}/configure-output.txt"

      fi

      (
        echo
        echo "Running gettext make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          make check # || true
        fi

        # Avoid strip here, it may interfere with patchelf.
        # make install-strip
        if false # [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gettext_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${gettext_src_folder_name}" \
        "${gettext_folder_name}"

    )

    touch "${gettext_stamp_file_path}"

  else
    echo "Library gettext already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_libelf()
{
  # http://www.mr511.de/ (?Deprecated?)
  # http://www.mr511.de/software/
  # https://sourceware.org/elfutils/
  # ftp://sourceware.org/pub/elfutils//0.178/elfutils-0.178.tar.bz2

  # https://archlinuxarm.org/packages/aarch64/libelf/files/PKGBUILD

  # libelf_version="0.8.13" (deprecated)
  # 26 Nov 2019, 0.178
  # 2020-03-30, 0.179
  # 2020-06-11, 0.180

  local libelf_version="$1"

  local libelf_src_folder_name="libelf-${libelf_version}"
  local libelf_archive="${libelf_src_folder_name}.tar.gz"

  # local libelf_url="http://www.mr511.de/software/${libelf_archive}"
  # The original site seems unavailable, use a mirror.
  local libelf_url="https://fossies.org/linux/misc/old/${libelf_archive}"

  local libelf_folder_name="${libelf_src_folder_name}"

  local libelf_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${libelf_folder_name}-installed"
  if [ ! -f "${libelf_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${libelf_url}" "${libelf_archive}" \
      "${libelf_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${libelf_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${libelf_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${libelf_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running libelf configure..."

          run_verbose bash "${SOURCES_FOLDER_PATH}/${libelf_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--disable-nls")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${libelf_src_folder_name}/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${libelf_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libelf_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running libelf make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make check
        fi

        run_verbose make install

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${libelf_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${libelf_src_folder_name}" \
        "${libelf_folder_name}"

    )

    touch "${libelf_stamp_file_path}"

  else
    echo "Library libelf already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_expat()
{
  # https://libexpat.github.io
  # https://github.com/libexpat/libexpat/releases

  # https://archlinuxarm.org/packages/aarch64/expat/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=expat-git

  # Oct 21, 2017 "2.1.1"
  # Nov 1, 2017 "2.2.5"
  # 26 Sep 2019 "2.2.9"

  local expat_version="$1"

  local expat_src_folder_name="expat-${expat_version}"
  local expat_archive="${expat_src_folder_name}.tar.bz2"
  if [[ "${expat_version}" =~ 2\.0\.* ]]
  then
    expat_archive="${expat_src_folder_name}.tar.gz"
  fi
  
  local expat_release="R_$(echo ${expat_version} | sed -e 's|[.]|_|g')"
  local expat_url="https://github.com/libexpat/libexpat/releases/download/${expat_release}/${expat_archive}"

  local expat_folder_name="${expat_src_folder_name}"

  local expat_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${expat_folder_name}-installed"
  if [ ! -f "${expat_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${expat_url}" "${expat_archive}" \
      "${expat_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${expat_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${expat_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${expat_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running expat configure..."

          run_verbose bash "${SOURCES_FOLDER_PATH}/${expat_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${expat_src_folder_name}/configure" \
            ${config_options[@]}
            
          cp "config.log" "${LOGS_FOLDER_PATH}/${expat_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${expat_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running expat make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make check
        fi

        run_verbose make install

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${expat_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${expat_src_folder_name}" \
        "${expat_folder_name}"

    )

    touch "${expat_stamp_file_path}"

  else
    echo "Library expat already installed."
  fi
}


# -----------------------------------------------------------------------------

function build_xz()
{
  # https://tukaani.org/xz/
  # https://sourceforge.net/projects/lzmautils/files/

  # https://archlinuxarm.org/packages/aarch64/xz/files/PKGBUILD
  # https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=xz-git

  # 2016-12-30 "5.2.3"
  # 2018-04-29 "5.2.4"

  local xz_version="$1"

  local xz_src_folder_name="xz-${xz_version}"
  local xz_archive="${xz_src_folder_name}.tar.xz"
  local xz_url="https://sourceforge.net/projects/lzmautils/files/${xz_archive}"

  local xz_folder_name="${xz_src_folder_name}"

  local xz_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${xz_folder_name}-installed"
  if [ ! -f "${xz_stamp_file_path}" ]
  then

    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${xz_url}" "${xz_archive}" \
      "${xz_src_folder_name}"

    mkdir -pv "${LOGS_FOLDER_PATH}/${xz_folder_name}"

    (
      mkdir -pv "${LIBS_BUILD_FOLDER_PATH}/${xz_folder_name}"
      cd "${LIBS_BUILD_FOLDER_PATH}/${xz_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS}"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running xz configure..."

          run_verbose bash "${SOURCES_FOLDER_PATH}/${xz_src_folder_name}/configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--disable-rpath")
          # config_options+=("--disable-nls")

          run_verbose bash ${DEBUG} "${SOURCES_FOLDER_PATH}/${xz_src_folder_name}/configure" \
            ${config_options[@]}

          cp "config.log" "${LOGS_FOLDER_PATH}/${xz_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${xz_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running xz make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make check
        fi

        if [ "${WITH_STRIP}" == "y" ]
        then
          run_verbose make install-strip
        else
          run_verbose make install
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${xz_folder_name}/make-output.txt"

      copy_license \
        "${SOURCES_FOLDER_PATH}/${xz_src_folder_name}" \
        "${xz_folder_name}"

    )

    touch "${xz_stamp_file_path}"

  else
    echo "Library xz already installed."
  fi
}

# -----------------------------------------------------------------------------

function build_gpm()
{
  # General purpose mouse. Used by ncurses.
  # https://www.nico.schottelius.org/software/gpm/
  # https://github.com/telmich/gpm
  # https://github.com/telmich/gpm/releases/tag/1.20.7
  # https://github.com/telmich/gpm/archive/1.20.7.tar.gz

  # https://archlinuxarm.org/packages/aarch64/gpm/files/PKGBUILD

  # 27 Oct 2012 "1.20.7"

  local gpm_version="$1"

  local gpm_src_folder_name="gpm-${gpm_version}"
  local gpm_archive="${gpm_src_folder_name}.tar.gz"
  local gpm_github_archive="${gpm_version}.tar.gz"
  
  local gpm_url="https://github.com/telmich/gpm/archive/${gpm_github_archive}"

  local gpm_folder_name="${gpm_src_folder_name}"

  local gpm_stamp_file_path="${INSTALL_FOLDER_PATH}/stamp-${gpm_folder_name}-installed"
  if [ ! -f "${gpm_stamp_file_path}" ]
  then

    # In-source build.

    cd "${LIBS_BUILD_FOLDER_PATH}"

    if [ ! -d "${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}" ]
    then
      cd "${LIBS_BUILD_FOLDER_PATH}"

      download_and_extract "${gpm_url}" "${gpm_archive}" \
        "${gpm_src_folder_name}"

      if [ "${gpm_src_folder_name}" != "${gpm_folder_name}" ]
      then
        mv -v "${gpm_src_folder_name}" "${gpm_folder_name}"
      fi
    fi

    mkdir -pv "${LOGS_FOLDER_PATH}/${gpm_folder_name}"

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}"
      if [ ! -f "stamp-autogen" ]
      then

        xbb_activate
        
        run_app bash ${DEBUG} "autogen.sh"

        touch "stamp-autogen"
      fi
    ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gpm_folder_name}/autogen-output.txt"

    (
      cd "${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}"

      xbb_activate
      xbb_activate_installed_dev

      CPPFLAGS="${XBB_CPPFLAGS} -I${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}/src/headers"
      CFLAGS="${XBB_CFLAGS_NO_W}"
      CXXFLAGS="${XBB_CXXFLAGS_NO_W}"
      LDFLAGS="${XBB_LDFLAGS_LIB}"
      if [ "${IS_DEVELOP}" == "y" ]
      then
        LDFLAGS+=" -v"
      fi

      export CPPFLAGS
      export CFLAGS
      export CXXFLAGS
      export LDFLAGS

      env | sort

      if [ ! -f "config.status" ]
      then 
        (
          echo
          echo "Running gpm configure..."

          run_verbose bash "configure" --help

          config_options=()

          config_options+=("--prefix=${LIBS_INSTALL_FOLDER_PATH}")
            
          config_options+=("--build=${BUILD}")
          config_options+=("--host=${HOST}")
          config_options+=("--target=${TARGET}")

          # config_options+=("--with-pic")

          run_verbose bash ${DEBUG} "configure" \
            ${config_options[@]}
            
          cp "config.log" "${LOGS_FOLDER_PATH}/${gpm_folder_name}/config-log.txt"
        ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gpm_folder_name}/configure-output.txt"
      fi

      (
        echo
        echo "Running gpm make..."

        # Build.
        run_verbose make -j ${JOBS}

        if [ "${WITH_TESTS}" == "y" ]
        then
          run_verbose make check
        fi

        run_verbose make install

        if [ "${TARGET_PLATFORM}" == "linux" ]
        then
          (
            mkdir -pv "${APP_PREFIX}/bin"
            cd "${APP_PREFIX}/bin"

            # Manual copy, since it is not refered in the elf.
            cp -v "${LIBS_INSTALL_FOLDER_PATH}/lib/libgpm.so.2.1.0" .
            rm -f "libgpm.so.2"
            ln -s -v "libgpm.so.2.1.0" "libgpm.so.2"
          )
        fi

      ) 2>&1 | tee "${LOGS_FOLDER_PATH}/${gpm_folder_name}/make-output.txt"

      copy_license \
        "${LIBS_BUILD_FOLDER_PATH}/${gpm_folder_name}" \
        "${gpm_folder_name}"
    )

    touch "${gpm_stamp_file_path}"

  else
    echo "Library gpm already installed."
  fi
}

# -----------------------------------------------------------------------------

# Download the Windows Python 2 libraries and headers.
function download_python2_win() 
{
  # https://www.python.org/downloads/release/python-2714/
  # https://www.python.org/ftp/python/2.7.14/python-2.7.14.msi
  # https://www.python.org/ftp/python/2.7.14/python-2.7.14.amd64.msi

  local python2_version="$1"

  local python2_version_major=$(echo ${python2_version} | sed -e 's|\([0-9]\)\..*|\1|')
  local python2_version_minor=$(echo ${python2_version} | sed -e 's|\([0-9]\)\.\([0-9][0-9]*\)\..*|\2|')

  if [ "${TARGET_BITS}" == "32" ]
  then
    PYTHON2_SRC_FOLDER_NAME=python-"${python2_version}"
  else
    PYTHON2_SRC_FOLDER_NAME=python-"${python2_version}".amd64
  fi

  # Used to compute GNURM_PYTHON_WIN_DIR.
  export PYTHON2_SRC_FOLDER_NAME

  local python2_win_pack="${PYTHON2_SRC_FOLDER_NAME}.msi"
  local python2_win_url="https://www.python.org/ftp/python/${python2_version}/${python2_win_pack}"

  cd "${SOURCES_FOLDER_PATH}"

  download "${python2_win_url}" "${python2_win_pack}"

  (
    xbb_activate

    if [ ! -d "${PYTHON2_SRC_FOLDER_NAME}" ]
    then
      cd "${SOURCES_FOLDER_PATH}"

      # Include only the headers and the python library and executable.
      echo '*.h' >"/tmp/included"
      echo 'python*.dll' >>"/tmp/included"
      echo 'python*.lib' >>"/tmp/included"
      7za x -o"${PYTHON2_SRC_FOLDER_NAME}" "${DOWNLOAD_FOLDER_PATH}/${python2_win_pack}" -i@"/tmp/included"

      # Patch to disable the macro that renames hypot.
      local patch_path="${BUILD_GIT_PATH}/patches/${PYTHON2_SRC_FOLDER_NAME}.patch"
      if [ -f "${patch_path}" ]
      then
        (
          cd "${PYTHON2_SRC_FOLDER_NAME}"
          patch -p0 <"${patch_path}" 
        )
      fi
    else
      echo "Folder ${PYTHON2_SRC_FOLDER_NAME} already present."
    fi

    echo "Copying python${python2_version_major}${python2_version_minor}.dll..."
    # From here it'll be copied as dependency.
    mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/bin/"
    install -v -c -m 644 "${PYTHON2_SRC_FOLDER_NAME}/python${python2_version_major}${python2_version_minor}.dll" \
      "${LIBS_INSTALL_FOLDER_PATH}/bin/"

    mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/lib/"
    install -v -c -m 644 "${PYTHON2_SRC_FOLDER_NAME}/python${python2_version_major}${python2_version_minor}.lib" \
      "${LIBS_INSTALL_FOLDER_PATH}/lib/"
  )
}

# -----------------------------------------------------------------------------

# Download the Windows Python 3 libraries and headers.
function download_python3_win() 
{
  # https://www.python.org/downloads/windows/
  # https://www.python.org/downloads/release/python-372/
  # https://www.python.org/ftp/python/3.7.2/python-3.7.2.post1-embed-win32.zip
  # https://www.python.org/ftp/python/3.7.2/python-3.7.2.post1-embed-amd64.zip
  # https://www.python.org/ftp/python/3.7.2/python-3.7.2.exe
  # https://www.python.org/ftp/python/3.7.2/python-3.7.2-amd64.exe
  # https://www.python.org/ftp/python/3.7.2/Python-3.7.2.tar.xz
  # https://www.python.org/ftp/python/3.7.6/
  # https://www.python.org/ftp/python/3.7.6/python-3.7.6-embed-amd64.zip
  # https://www.python.org/ftp/python/3.7.6/python-3.7.6-embed-win32.zip

  local python3_version="$1"
  
  local python3_version_major=$(echo ${python3_version} | sed -e 's|\([0-9]\)\..*|\1|')
  local python3_version_minor=$(echo ${python3_version} | sed -e 's|\([0-9]\)\.\([0-9][0-9]*\)\..*|\2|')

  # Version 3.7.2 uses a longer name, like python-3.7.2.post1-embed-amd64.zip.
  if [ "${TARGET_BITS}" == "32" ]
  then
    PYTHON3_WIN_EMBED_FOLDER_NAME=python-"${python3_version}-embed-win32"
  else
    PYTHON3_WIN_EMBED_FOLDER_NAME=python-"${python3_version}-embed-amd64"
  fi

  # Used in python3-config.sh
  export PYTHON3_WIN_EMBED_FOLDER_NAME
  export PYTHON3_SRC_FOLDER_NAME="Python-${python3_version}"

  local python3_win_embed_pack="${PYTHON3_WIN_EMBED_FOLDER_NAME}.zip"
  local python3_win_embed_url="https://www.python.org/ftp/python/${python3_version}/${python3_win_embed_pack}"

  (
    xbb_activate

    if [ ! -d "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_EMBED_FOLDER_NAME}" ]
    then
      mkdir -pv "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_EMBED_FOLDER_NAME}"
      cd "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_EMBED_FOLDER_NAME}"

      download_and_extract "${python3_win_embed_url}" "${python3_win_embed_pack}" "${PYTHON3_WIN_EMBED_FOLDER_NAME}"
    else
      echo "Folder ${PYTHON3_WIN_EMBED_FOLDER_NAME} already present."
    fi
      
    cd "${SOURCES_FOLDER_PATH}/${PYTHON3_WIN_EMBED_FOLDER_NAME}"
    echo "Copying python${python3_version_major}${python3_version_minor}.dll..."
    # From here it'll be copied as dependency.
    mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/bin/"
    install -v -c -m 644 "python${python3_version_major}.dll" \
      "${LIBS_INSTALL_FOLDER_PATH}/bin/"
    install -v -c -m 644 "python${python3_version_major}${python3_version_minor}.dll" \
      "${LIBS_INSTALL_FOLDER_PATH}/bin/"

    mkdir -pv "${LIBS_INSTALL_FOLDER_PATH}/lib/"
    install -v -c -m 644 "python${python3_version_major}.dll" \
      "${LIBS_INSTALL_FOLDER_PATH}/lib/"
    install -v -c -m 644 "python${python3_version_major}${python3_version_minor}.dll" \
      "${LIBS_INSTALL_FOLDER_PATH}/lib/"
  )

  local python3_archive="${PYTHON3_SRC_FOLDER_NAME}.tar.xz"
  local python3_url="https://www.python.org/ftp/python/${python3_version}/${python3_archive}"

  # local python3_folder_name="python-${python3_version}"

  if [ ! -d "${SOURCES_FOLDER_PATH}/${PYTHON3_SRC_FOLDER_NAME}" ]
  then
    cd "${SOURCES_FOLDER_PATH}"

    download_and_extract "${python3_url}" "${python3_archive}" \
      "${PYTHON3_SRC_FOLDER_NAME}"
  fi

  # The source archive includes only the pyconfig.h.in, which needs
  # to be configured, which is not an easy task. Thus add the file copied 
  # from a Windows install.
  cp -v "${BUILD_GIT_PATH}/patches/pyconfig-${python3_version}.h" \
    "${SOURCES_FOLDER_PATH}/${PYTHON3_SRC_FOLDER_NAME}/Include/pyconfig.h"
}

# -----------------------------------------------------------------------------

