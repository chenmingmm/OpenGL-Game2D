# fetchcontent重点来了
#-------------------------------------------------------------------
include(fetchcontent)
 fetchcontent_declare(glm	#库名字
						GIT_REPOSITORY https://github.com/g-truc/glm.git	# 仓库地址
						GIT_TAG 0.9.9.8 # 库版本
						SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glm # 指定库下载地址
						)

fetchcontent_makeavailable(glm)