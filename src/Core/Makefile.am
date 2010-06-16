noinst_LIBRARIES = libcore.a

libcore_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler

libcore_a_SOURCES = \
	memoryCell.cc \
	rhsBuilder.cc \
	termBag.cc \
	bindingLhsAutomaton.cc \
	trivialRhsAutomaton.cc \
	copyRhsAutomaton.cc \
	variableInfo.cc \
	narrowingVariableInfo.cc \
	termSet.cc \
	strategy.cc \
	sortTable.cc \
	sortConstraintTable.cc \
	equationTable.cc \
	ruleTable.cc \
	sort.cc \
	connectedComponent.cc \
	module.cc \
	substitution.cc \
	localBinding.cc \
	subproblemSequence.cc \
	subproblemDisjunction.cc \
	sortCheckSubproblem.cc \
	disjunctiveSubproblemAccumulator.cc \
	preEquation.cc \
	equation.cc \
	rule.cc \
	sortConstraint.cc \
	rewritingContext.cc \
	variableAbstractionSubproblem.cc \
	equalitySubproblem.cc \
	dagRoot.cc \
	cachedDag.cc \
	extensionMatchSubproblem.cc \
	dagNodeSet.cc \
	hashConsSet.cc \
	dagNodeCache.cc \
	memoTable.cc \
	rootContainer.cc \
	sortBdds.cc \
	unificationContext.cc \
	protectedDagNodeSet.cc \
	pendingUnificationStack.cc

EXTRA_DIST = \
	run.cc \
	ctorDiagram.cc \
	sortErrorAnalysis.cc

noinst_HEADERS = \
	argVec.hh \
	argumentIterator.hh \
	badFlag.hh \
	bindingLhsAutomaton.hh \
	cachedDag.hh \
	checkedArgVecConstIterator.hh \
	checkedArgVecIterator.hh \
	conditionFragment.hh \
	conditionState.hh \
	connectedComponent.hh \
	copyRhsAutomaton.hh \
	core.hh \
	dagArgumentIterator.hh \
	dagNodeSet.hh \
	hashConsSet.hh \
	dagNodeCache.hh \
	dagRoot.hh \
	disjunctiveSubproblemAccumulator.hh \
	equalitySubproblem.hh \
	equation.hh \
	equationTable.hh \
	extensionMatchSubproblem.hh \
	label.hh \
	lineNumber.hh \
	localBinding.hh \
	memoTable.hh \
	memoryCell.hh \
	module.hh \
	moduleItem.hh \
	namedEntity.hh \
	opDeclaration.hh \
	preEquation.hh \
	rawRootContainer.hh \
	redexPosition.hh \
	rewritingContext.hh \
	rhsBuilder.hh \
	rootContainer.hh \
	rule.hh \
	ruleTable.hh \
	simpleRootContainer.hh \
	sort.hh \
	sortBdds.hh \
	sortCheckSubproblem.hh \
	sortConstraint.hh \
	sortConstraintTable.hh \
	sortTable.hh \
	strategy.hh \
	subproblemAccumulator.hh \
	subproblemDisjunction.hh \
	subproblemSequence.hh \
	substitution.hh \
	symbolMap.hh \
	termBag.hh \
	termSet.hh \
	trivialRhsAutomaton.hh \
	variableAbstractionSubproblem.hh \
	variableInfo.hh \
	narrowingVariableInfo.hh \
	unificationContext.hh \
	freshVariableGenerator.hh \
	protectedDagNodeSet.hh \
	pendingUnificationStack.hh \
	environment.hh
