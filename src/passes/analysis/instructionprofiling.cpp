#include "coreir.h"
#include "coreir/passes/analysis/instructionprofiling.h"

using namespace CoreIR;

std::string Passes::InstructionProfiling::ID = "instructionprofiling";

uint get_bitwidth(Type* type) {
  uint bitwidth;
  Type* cType = type;

  // bitwidth is type right before base type
  while(!cType->isBaseType()) {
    if (auto aType = dyn_cast<ArrayType>(cType)) {
      bitwidth = aType->getLen();
      cType = aType->getElemType();
    } else {
      ASSERT(false, "not sure how to handle this type" + cType->toString());
    }
  }

  return bitwidth;
}

void Passes::InstructionProfiling::increment(Instance* inst) {

  auto moduleref = inst->getModuleRef();
  std::string instname = moduleref->getRefName();
  //get_bitwidth(inst->sel("out")->getType());
  uint bitwidth = moduleref->isGenerated() ?
      (moduleref->getGenArgs().count("width") ?
       moduleref->getGenArgs().at("width")->get<int>() :
       (moduleref->getGenArgs().count("bitwidth") ?
        moduleref->getGenArgs().at("width")->get<int>() : 0)) : 0;

  instruction_counts[instname][bitwidth] += 1;
  //std::cout << "incremented for " << instname << " with bitwidth=" << bitwidth << std::endl;
}

bool Passes::InstructionProfiling::runOnInstanceGraphNode(InstanceGraphNode& node) {
  Module* m = node.getModule();
  if (!m->hasDef()) return false;
  ModuleDef* def = m->getDef();

  // count all instances
  for (auto instpair : def->getInstances()) {
    Instance* inst = instpair.second;
    increment(inst);
  }

  writeToStream(std::cout);
  
  return false;
}

void Passes::InstructionProfiling::writeToStream(std::ostream& os) {
  for (auto instname_bitwidthmap_pair : instruction_counts) {
    std::string instname = instname_bitwidthmap_pair.first;
    auto inst_bw_map = instname_bitwidthmap_pair.second;

    for (auto bitwidth_count_pair : inst_bw_map) {
      uint bitwidth = bitwidth_count_pair.first;
      int count = bitwidth_count_pair.second;
      if (bitwidth == 0) {
        os << instname << ": " << count << std::endl;
      } else {
        os << instname << bitwidth << ": " << count << std::endl;
      }
    }
  }
}
