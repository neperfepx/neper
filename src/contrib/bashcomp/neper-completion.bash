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
  # e.g., -clface and -cl
  opts_T="-n -id -morpho -ori -morphooptialgo -orioptialgo -morphooptiini \
  -morphooptialgoini -morphooptiobjective -morphooptigrid -morphooptismooth \
  -morphooptideltamax -morphooptiboundlower -morphooptiboundupper -morphooptiinistep \
  -morphooptistop -morphooptilogtime -morphooptilogvar -morphooptilogdis \
  -morphooptilogtesr -morphooptilogval -morphooptidof -morphooptimultiseed \
  -morphosampling -orioptidof -orisampling -orioptiinistep -orioptideltamax \
  -orioptiboundlower -orioptiboundupper -orioptistop -orioptineigh -orioptiini \
  -orioptifix -oridescriptor -oricrysym -oriformat -orioptilogvar -orioptilogval \
  -orispread -dim -periodicity -o -v -libpath -domain -domainfacet -transform -group \
  -reg -regfmax -regsel -regrsel -regloop -regedgedelmax -regdelcell \
  -format -tesrformat -tesrsize \
  -crysym -stattess -stattesr -statver -statedge -statface -statpoly -statcell -statseed \
  -statpoint -statvox -statgroup -sort -loadtess -loadtesr -loadpoint -checktess "

  opts_load_T="-loadtesr -loadtess -loadpoint "

  opts_M="-o -v -elttype -cl -rcl -clpoly -rclpoly -clface -rclface -cledge -rcledge \
  -clver -rclver -clratio -pl -plpoly -plface -pledge -clmin -order -dim \
  -meshreconstruct -mesh2dalgo -mesh2dpinchfix -mesh3dalgo -meshqualexpr \
  -meshqualdisexpr -meshqualmin -singnodedup -dupnodemerge -tesrsmooth -tesrsmoothfact \
  -tesrsmoothitermax -meshedge -meshface -meshpoly -interface -mesh3dreport -transform \
  -sortelt -sortnode -part -format -elset -nset -faset -performat -statnode \
  -statmesh -statmesh0d -statmesh1d -statmesh2d -statmesh3d -statmeshco -statelt \
  -statelt0d -statelt1d -statelt2d -statelt3d -stateltco -statelset -statelset0d \
  -statelset1d -statelset2d -statelset3d -statelsetco -statpoint -mesh2dmaxtime \
  -mesh2drmaxtime -mesh2diter -mesh3dmaxtime -mesh3drmaxtime -mesh3diter -mesh3dclreps \
  -transport -transporteltmethod -loadtess -loadmesh -loadpoint "

  opts_load_M="-loadmesh -loadpoint "

  opts_S="-o -orispace -entity -step -resnode -reselt -reselset -resmesh -rescell \
  -restess "

  opts_V="-povray -asymptote -simstep -step -crysym -space -slicemesh -cameracoo \
  -cameralookat -camerasky -cameraangle -cameraprojection -lightsource -lightambient \
  -lightdiffuse -lightreflection -scenebackground -pfpole -pfpolelabel \
  -pfprojection -pfprojectionlabel -pfmode -pffont -pfgridsize -pfkernel -pfdir -pfsym \
  -pfclustering -pfshape -oritrs -oricol -oriedgecol -oriedgerad -oriedgetrs -orimode \
  -orilayout -orifield -ipfpole -ipfpolelabel -ipfprojection -ipfprojectionlabel \
  -ipfmode -ipffont -ipfgridsize -ipfkernel -ipfdir -ipfsym -ipfclustering -ipfshape \
  -imageformat -imagesize -includepov -print -datacell \
  -datacellcol -datapolycol -datafacecol -dataedgecol -datavercol -dataseedcol \
  -datacrystalcol -datavoxcol -datavoidvoxcol -datacrystaledgecol -datacelledgecol \
  -datavoxedgecol -datacellcolscheme -datapolycolscheme -datafacecolscheme \
  -dataedgecolscheme -datavercolscheme -dataseedcolscheme -datacrystalcolscheme \
  -datavoxcolscheme -datacelltrs -datapolytrs -datafacetrs -dataedgetrs -datavertrs \
  -dataseedtrs -datacrystaltrs -datacellrad -datapolyrad -datafacerad -dataedgerad \
  -dataverrad -dataseedrad -datacrystalrad -datacrystaledgerad -datacelledgerad \
  -datavoxedgerad -datacellscale -datapolyscale -datafacescale -dataedgescale \
  -dataverscale -dataseedscale -datacrystalscale -datavoxscale -datacellscaletitle \
  -datapolyscaletitle -datafacescaletitle -dataedgescaletitle -dataverscaletitle \
  -dataseedscaletitle -datacrystalscaletitle -datavoxscaletitle -datacellsymbol \
  -dataelset -dataelsetcol -dataeltcol -dataelt0dcol -dataelt1dcol -dataelt2dcol \
  -dataelt3dcol -dataelset0dcol -dataelset1dcol -dataelset2dcol -dataelset3dcol \
  -datanodecol -datameshcol -dataelt2dedgecol -dataelt3dedgecol -dataeltcolscheme \
  -dataelt0dcolscheme -dataelt1dcolscheme -dataelt2dcolscheme -dataelt3dcolscheme \
  -dataelsetcolscheme -dataelset0dcolscheme -dataelset1dcolscheme -dataelset2dcolscheme \
  -dataelset3dcolscheme -dataeltscale -dataelt0dscale -dataelt1dscale -dataelt2dscale \
  -dataelt3dscale -dataelsetscale -dataelset0dscale -dataelset1dscale -dataelset2dscale \
  -dataelset3dscale -dataeltscaletitle -dataelt0dscaletitle -dataelt1dscaletitle \
  -dataelt2dscaletitle -dataelt3dscaletitle -dataelsetscaletitle -dataelset0dscaletitle \
  -dataelset1dscaletitle -dataelset2dscaletitle -dataelset3dscaletitle -dataelt0drad \
  -dataelt1drad -datanoderad -dataelt2dedgerad -dataelt3dedgerad -dataelset0drad \
  -dataelset1drad -dataelset2drad -dataelset3drad -datanodecoo -datanodecoofact \
  -datacsyscoo -datacsyslength -datacsysrad -datacsyslabel -datacsyscol \
  -datacsysfontsize -showtess -showtesr -showmesh -showmeshslice -showcsys -showscale \
  -showscalemin -showscalemax -showcell -showpoly -showface -showedge -showver -showseed \
  -showcrystal -showfaceinter -showvox -showelt -showelt0d -showelt1d -showelt2d \
  -showelt3d -showelset -showelset0d -showelset1d -showelset2d -showelset3d -shownode \
  -dataelset2dedgerad -dataelset2dedgecol -dataelset3dedgerad -dataelset3dedgecol \
  -datanodecolscheme -datanodescale -datanodescaletitle -datapointcoo -datapointcoofact \
  -datapointrad -datapointcol -datapointcolscheme -datapointtrs -datapointedgerad \
  -datapointedgecol -datapointscale -datapointscaletitle -showpoint -showvoidvox -loop \
  -endloop "


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
