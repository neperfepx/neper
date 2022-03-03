#/usr/bin/env bash

# This script is used to add bash completion feature to the Neper software
# Author: Xiao Shang (93xshang@gmail.com)

_neper_completions() {

  if [[ $COMP_WORDBREAKS == *, ]]; then
    COMP_WORDBREAKS="${COMP_WORDBREAKS//,}"
  fi

  local cur prev opts opts_M opts_S opts_V opts_T
  COMPREPLY=()
  cur="${COMP_WORDS[COMP_CWORD]}"
  prev="${COMP_WORDS[COMP_CWORD-1]}"
  opts="-T -M -S -V --help --rcfile --version --license"
  # In the following strings, always put a space at last to differentiate,
  # e.g., -partmethod and -part
  opts_T="-n -id -dim -domain -periodicity -loadtess -loadtesr -statpoint -loadpoint \

  -morpho -morphooptiini -morphooptiobjective -morphooptidof -morphooptistop \
  -morphooptialgo -morphooptigrid -morphooptismooth -morphooptideltamax \
  -morphooptiinistep -morphooptilogtime -morphooptilogvar -morphooptilogval \
  -morphooptilogdis -morphooptilogtesr -morphooptialgomaxiter -group \

  -ori -oridistrib -oricrysym -orioptiini -orioptifix -orioptistop -orioptineigh \
  -orioptilogvar \

  -transform -sort \

  -regularization -fmax -sel -rsel -mloop -o -format -tesrformat -tesrsize \
  -oridescriptor -oriformat \

  -stattess -stattesr -statcell -statseed -statgroup -statver -statedge -statface \
  -statpoly -statvox -statpoint \

  -checktess "

  opts_load_T="-loadtesr -loadtess -loadpoint "

  opts_M="-gmsh -tmp \

  -loadmesh -loadpoint \

  -elttype -cl -rcl -dim -order -clface -rclface -cledge -rcledge -clver -rclver \
  -pl -clratio -clmin -meshqualmin -meshqualexpr -meshqualdisexpr -mesh2dalgo \
  -mesh3dalgo -interface -mesh2dpinchfix \

  -tesrsmooth -tesrsmoothfact -tesrsmoothitermax \

  -clean -singnodedup -dupnodemerge \

  -transform -sortnode -sortelt \

  -part -partmethod \

  -transport -transporteltmethod \

  -o -format -nset -faset -elset -performat \

  -statmesh -statmesh0d -statmesh1d -statmesh2d -statmesh3d -statmeshco -statnode\
  -statelt -statelt0d -statelt1d -statelt2d -statelt3d -statelset -statelset0d \
  -statelset1d -statelset2d -statelset3d -statpoint \

  -mesh3dclreps -mesh2dmaxtime -mesh2drmaxtime -mesh2diter -mesh3dmaxtime \
  -mesh3drmaxtime -mesh3diter "

  opts_load_M="-loadmesh -loadpoint "

  opts_S="-resnode -reselt -reselset -resmesh -rescell -restess \

  -o "

  opts_V="-povray \

  -simstep \

  -space \

  -datacellcol -datacellcolscheme -datacelltrs -datacellscale -datacellscaletitle \
  -datapolycol -datapolycolscheme -datapolytrs -datapolyscale -datapolyscaletitle \
  -datafacecol -datafacecolscheme -datafacetrs -datafacescale -datafacescaletitle \
  -dataedgerad -dataedgecol -dataedgecolscheme -dataedgetrs -dataedgescale \
  -dataedgescaletitle -dataverrad -datavercol -datavercolscheme -datavertrs \
  -dataverscale -dataverscaletitle -dataseedrad -dataseedtrs -dataseedcol -dataseedcolscheme \
  -dataseedscale -dataseedscaletitle -datacrystalrad -datacrystaltrs -datacrystalcol
  -datacrystalcolscheme -datacrystalscale -datacrystalscaletitle \
  -datavoxcol -datavoxcolscheme -datavoxscale \
  -datavoxscaletitle -datavoxedgerad -datavoxedgecol -datavoidvoxcol -dataedgerad \
  -dataelt3dcol -dataelt3dcolscheme -dataelt3dscale -dataelt3dscaletitle \
  -dataelt3dedgerad -dataelt3dedgecol \
  -dataelt2dcol -dataelt2dcolscheme -dataelt2dscale -dataelt2dscaletitle \
  -dataelt2dedgerad -dataelt2dedgecol \
  -dataelt1dcol -dataelt1dcolscheme -dataelt1dscale -dataelt1dscaletitle -dataelt1drad \
  -dataelt0dcol -dataelt0dcolscheme -dataelt0dscale -dataelt0dscaletitle -dataelt0drad \
  -dataelset3dcol -dataelset3dcolscheme -dataelset3dscale -dataelset3dscaletitle \
  -dataelset3dedgerad -dataelset3dedgecol \
  -dataelset2dcol -dataelset2dcolscheme -dataelset2dscale -dataelset2dscaletitle \
  -dataelset2dedgerad -dataelset2dedgecol \
  -dataelset1dcol -dataelset1dcolscheme -dataelset1dscale -dataelset1dscaletitle -dataelset1drad \
  -dataelset0dcol -dataelset0dcolscheme -dataelset0dscale -dataelset0dscaletitle -dataelset0drad \
  -datanodecoo -datanodecoofact -datanoderad -datanodecol -datanodecolscheme \
  -datanodescale -datanodescaletitle \

  -datapointcoo -datapointcoofact -datapointrad -datapointcol -datapointcolscheme \
  -datapointtrs -datapointedgerad -datapointedgecol -datapointscale -datapointscaletitle \

  -datacsyscoo -datacsyslength -datacsysrad -datacsyslabel -datacsyscol \

  -slicemesh \

  -showtess -showtesr -showmesh -showmeshslice -showpoint -showcell -showpoly \
  -showface -showedge -showver -showseed -showcrystal -showfaceinter -showvox -showvoidvox \
  -showedge -showelt3d -showelt2d -showelt1d -showelt0d -shownode -showcsys \

  -cameracoo -cameralookat -cameraangle -camerasky -cameraprojection \

  -imagesize -imageantialias -imageformat \

  -sceneshadow -scenebackground \

  -loop -endloop \

  -outdir \

  -print \

  -includepov "


  if [[ ${COMP_CWORD} -eq 1 ]]; then
#  if [[ ${cur} == -* || ${COMP_CWORD} -eq 1 ]] && ! [[ ${opts} =~ "${COMP_WORDS[1]} " ]]; then
    COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
    return 0
  fi

  #local word2rm # Used for "selective completion"
  case ${COMP_WORDS[1]} in
    -T)
      # Commented below are for "selective completion". True for all four modules
      # for word2rm in ${COMP_WORDS[@]}
      # do
      #   if [[ ${opts_T} =~ "$word2rm " ]]; then
      #     opts_T="${opts_T//$word2rm /}"
      #   fi
      # done
      if [[ ${prev} != -* ]]; then
        COMPREPLY=( $(compgen -W "${opts_T}" -- ${cur}) )
      elif [[ ${cur} == -* ]]; then
        COMPREPLY=( $(compgen -W "${opts_load_T}" -- ${cur}) )
      fi
      ;;

    -M)
      # for word2rm in ${COMP_WORDS[@]}
      # do
      #   if [[ ${opts_M} =~ "$word2rm " ]]; then
      #     opts_M="${opts_M//$word2rm /}"
      #   fi
      # done
      if [[ ${COMP_CWORD} -eq 2 ]] && [[ ${cur} == -* ]]; then
        COMPREPLY=( $(compgen -W "${opts_load_M}" -- ${cur}) )
      elif [[ ${COMP_CWORD} -eq 2 ]] || [[ $cur == *, ]]; then
        COMP_WORDBREAKS=${COMP_WORDBREAKS},
      elif [[ ${prev} != -* ]]; then
        COMPREPLY=( $(compgen -W "${opts_M}" -- ${cur}) )
      fi
      ;;

    -S)
      #for word2rm in ${COMP_WORDS[@]}
      # do
      #   if [[ ${opts_S} =~ "$word2rm " ]]; then
      #     opts_M="${opts_S//$word2rm /}"
      #   fi
      # done
      if [[ ${COMP_CWORD} -eq 2 ]] || [[ $cur == *, ]]; then
        COMP_WORDBREAKS=${COMP_WORDBREAKS},
      elif [[ ${prev} != -* ]]; then
        COMPREPLY=( $(compgen -W "${opts_S}" -- ${cur}) )
      fi
      ;;

    -V)
      # for word2rm in ${COMP_WORDS[@]}
      # do
      #   if [[ ${opts_V} =~ "$word2rm " ]]; then
      #     opts_M="${opts_V//$word2rm /}"
      #   fi
      # done
      if [[ ${COMP_CWORD} -eq 2 ]] || [[ $cur == *, ]]; then
        COMP_WORDBREAKS=${COMP_WORDBREAKS},
      elif [[ ${prev} != -* ]]; then
        COMPREPLY=( $(compgen -W "${opts_V}" -- ${cur}) )
      fi
      ;;
  esac
}

# rquey 07 Feb 2022: removed "-o nospace"
complete -o default -F _neper_completions neper
