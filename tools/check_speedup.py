# This script helps to check the speedup (and noise) of a benchmark
# by building and running a solution against the baseline code.

# Requirement: guard the code of the solution with #ifdef SOLUTION

# Example usage:
# $ mkdir check_speedup
# $ cd check_speedup
# $ python "C:\workspace\perf-ninja\tool\check_noise.py"
#   -lab_path "C:\workspace\perf-ninja\labs\misc\warmup"
#   -DSOLUTION -num_runs 3 -v

import subprocess
import os
import argparse
from pathlib import Path
import shutil
import gbench
from gbench import util, report
from gbench.util import *

parser = argparse.ArgumentParser(description='test results')
parser.add_argument("-num_runs", type=int, help="Number of runs", default=5)
parser.add_argument("-lab_path", type=str, help="Path to the root of the lab.")
parser.add_argument("-D", type=str, help="Path to the root of the lab.", default="SOLUTION")
parser.add_argument("-v", help="verbose", action="store_true", default=False)

args = parser.parse_args()
numRuns = args.num_runs
labRootPath = args.lab_path
defines = args.D
verbose = args.v

saveCWD = os.getcwd()

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def callWrapper(cmdLine):
  if verbose:
    print(bcolors.OKCYAN + "Running: " + cmdLine + bcolors.ENDC)
    subprocess.check_call(cmdLine, shell=True)
  else:
    subprocess.check_call(cmdLine, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, shell=True)
  return

def buildAndRunBench(iterNumber, variant, cmakeFlags):
  os.chdir(saveCWD)
  buildPath = os.path.join(saveCWD, variant + "Build" + str(iterNumber))
  buildDirPath = Path(buildPath)
  if buildDirPath.exists() and buildDirPath.is_dir():
    shutil.rmtree(buildDirPath)
  try:
    callWrapper("cmake -E make_directory " + variant +"Build" + str(iterNumber))
    os.chdir(buildPath)
    labAbsPath = labRootPath
    if not os.path.isabs(labAbsPath):
      labAbsPath = os.path.join(saveCWD, labRootPath)
    callWrapper("cmake -B . -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_PREFIX_PATH=/media/s2000/PerformanceNinja/local -DCMAKE_BUILD_TYPE=Release " + cmakeFlags + " -S \"" + labAbsPath + "\"")
    callWrapper("cmake --build . --config Release --parallel 8")
    # this will save score in result.json file
    callWrapper("cmake --build . --config Release --target validateLab")
    callWrapper("cmake --build . --config Release --target benchmarkLab")
  except:
    print(bcolors.FAIL + variant + ": iteration " + str(iterNumber) + " - Failed" + bcolors.ENDC)
    return False
  print(bcolors.OKGREEN + variant + ": iteration " + str(iterNumber) + " - Done" + bcolors.ENDC)
  return True

def compareResults(iterNumber):
  baselineBuildPath = os.path.join(saveCWD, "baselineBuild" + str(iterNumber))
  baselineResultPath = os.path.join(baselineBuildPath, "result.json")
  solutionBuildPath = os.path.join(saveCWD, "solutionBuild" + str(iterNumber))
  solutionResultPath = os.path.join(solutionBuildPath, "result.json")
  facitBuildPath = os.path.join(saveCWD, "facitBuild" + str(iterNumber))
  facitResultPath = os.path.join(facitBuildPath, "result.json")

  outJsonBaseline = gbench.util.load_benchmark_results(baselineResultPath)
  outJsonSolution = gbench.util.load_benchmark_results(solutionResultPath)
  outJsonFacit    = gbench.util.load_benchmark_results(facitResultPath)

  print(f"\n\n Iteration {iterNumber}:")

  def compare(oldName, oldJson, newName, newJson):
    diff_report = gbench.report.get_difference_report(
      oldJson, newJson, True)
    output_lines = gbench.report.print_difference_report(
      diff_report, False, True, 0.05, True)
    print(f"\n{oldName} to {newName}:")
    for line in output_lines:
      print(line)

  compare("Baseline", outJsonBaseline, "Solution", outJsonSolution)
  compare("Baseline", outJsonBaseline, "Facit",    outJsonFacit)
  compare("Facit",    outJsonFacit,    "Solution", outJsonSolution)

  return True

for i in range(0, numRuns):
  buildAndRunBench(i, "baseline", "-DCMAKE_CXX_FLAGS=-D" + defines + "=0")
  buildAndRunBench(i, "solution", "-DCMAKE_CXX_FLAGS=-D" + defines + "=1")
  buildAndRunBench(i, "facit",    "-DCMAKE_CXX_FLAGS=-D" + defines + "=2")

os.chdir(saveCWD)

for i in range(0, numRuns):
  compareResults(i)
