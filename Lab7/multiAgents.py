# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).
import math
from argparse import Action

from util import manhattanDistance
from game import Directions
import random, util

from game import Agent
from pacman import GameState


class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """

    def getAction(self, gameState: GameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {NORTH, SOUTH, WEST, EAST, STOP}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices)  # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState: GameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"

        result = 0
        if action == 'Stop':
            result -= 1000

        capsules = currentGameState.getCapsules()
        foodList = currentGameState.getFood().asList()
        ghostList = successorGameState.getGhostPositions()
        ghostPositions = currentGameState.getGhostPositions()
        scaredTimes = [ghostState.scaredTimer for ghostState in currentGameState.getGhostStates()]

        currentCapsuleDistance = 100
        for capsule in capsules:
            currentCapsuleDistance = min(currentCapsuleDistance, manhattanDistance(newPos, capsule))
        result += 100 - currentCapsuleDistance

        currentFoodDistance = 100
        for food in foodList:
            currentFoodDistance = min(currentFoodDistance, manhattanDistance(newPos, food))
        result += 100 - currentFoodDistance

        if scaredTimes[0] < 2:
            for ghost in ghostList:
                if manhattanDistance(newPos, ghost) < 3:
                    result -= 10000
        else:
            result += (100 - RealDistance(newPos, ghostPositions[0])) * 10

        return result


def RealDistance(pos1, pos2):
    return math.sqrt((pos1[0] - pos2[0]) ** 2 + (pos1[1] - pos2[1]) ** 2)


def scoreEvaluationFunction(currentGameState: GameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()


class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn='scoreEvaluationFunction', depth='2'):
        self.index = 0  # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)


class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
        Returns a list of legal actions for an agent
        agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
        Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
        Returns the total number of agents in the game

        gameState.isWin():
        Returns whether or not the game state is a winning state

        gameState.isLose():
        Returns whether or not the game state is a losing state
        """
        "*** YOUR CODE HERE ***"
        return self.max(gameState, 0)

    def max(self, gameState, depth):
        if depth == self.depth or gameState.isWin() or gameState.isLose():
            return self.evaluationFunction(gameState)
        if len(gameState.getLegalActions()) == 0:
            return self.evaluationFunction(gameState)
        v = float('-inf')
        ac = Directions.STOP
        for a in gameState.getLegalActions():
            s = self.min(gameState.generateSuccessor(0, a), depth, 1)
            if s > v:
                v = s
                ac = a
        if depth == 0:
            return ac
        else:
            return v

    def min(self, gameState, depth, agentID):
        if len(gameState.getLegalActions()) == 0:
            return self.evaluationFunction(gameState)
        v = float('inf')
        nextAgent = agentID + 1
        for a in gameState.getLegalActions(agentID):
            if nextAgent == gameState.getNumAgents():
                s = self.max(gameState.generateSuccessor(agentID, a), depth + 1)
            else:
                s = self.min(gameState.generateSuccessor(agentID, a), depth, nextAgent)
            v = min(s, v)
        return v


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        return self.max(gameState, 0, float('-inf'), float('inf'))

    def max(self, gameState, depth, alpha, beta):
        if depth == self.depth or gameState.isWin() or gameState.isLose():
            return self.evaluationFunction(gameState)
        if len(gameState.getLegalActions()) == 0:
            return self.evaluationFunction(gameState)
        v = float('-inf')
        ac = Directions.STOP
        for a in gameState.getLegalActions():
            s = self.min(gameState.generateSuccessor(0, a), depth, 1, alpha, beta)
            if s > v:
                v = s
                ac = a
            if v > beta:
                return v
            alpha = max(alpha, v)
        if depth == 0:
            return ac
        else:
            return v

    def min(self, gameState, depth, agentID, alpha, beta):
        if len(gameState.getLegalActions()) == 0:
            return self.evaluationFunction(gameState)
        v = float('inf')
        nextAgent = agentID + 1
        for a in gameState.getLegalActions(agentID):
            if nextAgent == gameState.getNumAgents():
                s = self.max(gameState.generateSuccessor(agentID, a), depth + 1, alpha, beta)
            else:
                s = self.min(gameState.generateSuccessor(agentID, a), depth, nextAgent, alpha, beta)
            v = min(s, v)
            if (v < alpha):
                return v
            beta = min(beta, v)
        return v


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "*** YOUR CODE HERE ***"
        return self.max(gameState, 0)

    def max(self, gameState, depth):
        if depth == self.depth or gameState.isWin() or gameState.isLose():
            return self.evaluationFunction(gameState)
        if len(gameState.getLegalActions()) == 0:
            return self.evaluationFunction(gameState)
        v = float('-inf')
        ac = Directions.STOP
        for a in gameState.getLegalActions():
            s = self.min(gameState.generateSuccessor(0, a), depth, 1)
            if s > v:
                v = s
                ac = a
        if depth == 0:
            return ac
        else:
            return v

    def min(self, gameState, depth, agentID):
        if len(gameState.getLegalActions()) == 0:
            return self.evaluationFunction(gameState)
        v = 0
        nextAgent = agentID + 1
        p = 1 / len(gameState.getLegalActions(agentID))
        for a in gameState.getLegalActions(agentID):
            if nextAgent == gameState.getNumAgents():
                s = self.max(gameState.generateSuccessor(agentID, a), depth + 1)
            else:
                s = self.min(gameState.generateSuccessor(agentID, a), depth, nextAgent)
            v += p * s
        return v


def betterEvaluationFunction(currentGameState: GameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    if currentGameState.isWin():
        return float('inf')
    if currentGameState.isLose():
        return float('-inf')

    result = 0
    pos = currentGameState.getPacmanPosition()
    foodList = currentGameState.getFood().asList()
    ghostList = currentGameState.getGhostPositions()
    ghostPositions = currentGameState.getGhostPositions()




    currentFoodDistance = 1000
    if (len(foodList) == 0):
        currentFoodDistance = 0
    else:
        result += 100000 / len(foodList)
    for food in foodList:
        currentFoodDistance = min(currentFoodDistance, RealDistance(pos, food))
    result -= currentFoodDistance**3

    for ghost in ghostList:
            result -= 10**(3-manhattanDistance(pos, ghost))


    return result


# Abbreviation
better = betterEvaluationFunction
