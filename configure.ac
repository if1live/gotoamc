dnl 시작매크로
AC_INIT(amc, 0.0.1)
AM_INIT_AUTOMAKE(amc, 0.0.1)

dnl 체크할 프로그램
AC_PROG_CXX
AC_PROG_CXXCPP

dnl 체크할 라이브러리
AC_CHECK_LIB([avcodec], [avcodec_alloc_context])
AC_CHECK_LIB([avformat], [av_register_all])
AC_CHECK_LIB([swscale], [sws_getContext])

dnl 체크할 헤더파일
AC_CHECK_HEADERS([avcodec.h avformat.h swscale.h])

dnl 최종적으로 생성할 파일명
AC_CONFIG_FILES([Makefile])
AC_OUTPUT