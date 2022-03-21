import numpy as np

from dataclasses import dataclass, field
from typing import List

rng = np.random.default_rng(4385)


class Neural:
    def __init__(self, max_error=0.01, lr=0.2, epochs=10000):
        self.max_error = max_error
        self.error = None
        self.target = None
        self.lr = lr  # Learning rate
        self.trained = True
        self.track_error = []
        self.epochs = epochs

        self.hid_w = rng.uniform(size=(2, 2))
        self.out_w = rng.uniform(size=(1, 2))

        self.hid_bias = rng.uniform(size=(1, 2))
        self.out_bias = rng.uniform()

    def init_layers(self, in1, in2):
        self.inp = Layer([Node(), Node()])
        self.inp.set("y", in1, in2)
        self.target = np.logical_xor(in1, in2)
        self.hid = Layer([Node(), Node()])
        self.out = Layer([Node()])

    def forward_pass(self):
        # Calculate hidden layer activation
        hid_u = (self.inp.get("y") @ self.hid_w + self.hid_bias).flatten()
        self.hid.set("u", *hid_u)
        # Calculate out layer activation
        out_u = (self.hid.get("y") @ self.out_w.T + self.out_bias).flatten()
        self.out.set("u", *out_u)

        self.error = 0.5 * np.power(self.out.get("y") - self.target, 2)

    def backpropagate(self):
        # Calculate deltas
        out_d = (self.out.get("delta") * (self.target - self.out.get("y"))).flatten()
        self.out.set("delta", out_d)
        hid_d = (self.hid.get("delta") * (out_d @ self.out_w)).flatten()
        self.hid.set("delta", *hid_d)
        # Update hidden weights and bias
        self.inp.set("delta", [0, 0])

        hid_w_upd = self.inp.get("y").T @ self.hid.get("delta") * self.lr
        self.hid_w += hid_w_upd
        hid_b_upd = self.hid.get("delta") * self.lr
        self.hid_bias += hid_b_upd

        # Update output weights and bias
        out_w_upd = self.hid.get("y") * self.out.get("delta") * self.lr
        self.out_w += out_w_upd
        out_b_upd = self.lr * self.out.get("delta")
        self.out_bias += out_b_upd

    def write_to_file(self):
        with open(f"trained_weights{self.lr}-{self.max_error}.txt", "w+") as f:
            np.savetxt(f, self.hid_w)
            np.savetxt(f, self.hid_bias)
            np.savetxt(f, self.out_w)
            np.savetxt(f, self.out_bias)

    def run(self):
        inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
        self.trained = True
        for inpt in inputs:
            self.init_layers(*inpt)
            self.forward_pass()
            # Track [0, 0] input error
            if (inpt == [0, 0]).all():
                self.track_error.append(self.error[0])
            if self.error < self.max_error:
                continue
            self.trained = False
            self.backpropagate()

    def run_epochs(self):
        for i in range(self.epochs):
            self.run()
            if self.trained == True:
                print(
                    f"Trained after {i} epochs with target mean squared loss {self.max_error:.4f} and LR={self.lr:.2f}"
                )
                self.write_to_file()
                return
        print(f"Neural Network did not converge after {i} epochs with LR={self.lr:.2f} "
              f" and max squared loss {self.max_error}. Current error : {self.error[0][0]:.4f}")

    def run_w_check(self):
        self.run_epochs()
        inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])

        for inpt in inputs:
            self.init_layers(*inpt)
            self.forward_pass()
            print(f"Checking for [{(*inpt,)}]")
            print(f"Target: {int(self.target)} Out {self.out.nodes[0].y}")


@dataclass(eq=False)
class Node:
    u: float = 0
    y: float = field(init=False)
    delta: float = field(init=False)

    def __post_init__(self):
        self.y = self.actf()
        self.delta = self.actf2()

    def __setattr__(self, attr, value):
        # When `u` value is changed, update `y` and `delta`
        if attr == "u":
            self.__dict__[attr] = value
            self.__dict__["y"] = self.actf()
            self.__dict__["delta"] = self.actf2()
        elif attr == "y":
            # When `y` is changed, update `delta`
            self.__dict__["delta"] = self.actf2()
        super().__setattr__(attr, value)

    def actf(self):
        # Activation function
        return 1 / (1 + np.exp(-self.u))

    def actf2(self):
        return self.y * (1 - self.y)


@dataclass(eq=False)
class Layer:
    nodes: List[Node]

    def get(self, attr):
        return np.array([getattr(n, str(attr)) for n in self.nodes]).reshape(
            1, len(self.nodes)
        )

    def set(self, attr, *args):
        if type(args[0]) == np.array or type(args[0]) == list:
            # Untuple if np.array or list
            (args,) = args
        for i, arg in enumerate(args):
            setattr(self.nodes[i], str(attr), arg)


if __name__ == "__main__":
    Neural().run_w_check()
